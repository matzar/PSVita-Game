/*
*
* Author: Chris Campbell - www.iforce2d.net
*
* Copyright (c) 2006-2011 Erin Catto http://www.box2d.org
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*/

#ifndef IFORCE2D_PHYSICSDRIVENPARTICLES_H
#define IFORCE2D_PHYSICSDRIVENPARTICLES_H

#include <vector>
#include <set>
using namespace std;

#ifndef DEGTORAD
#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f
#endif

#define MAX_PARTICLES 100

//keep this outside the test class so that
//it does not change when user hits 'R'
bool g_followCar = true;

enum _controlState {
    CS_LEFT     = 0x01,
    CS_RIGHT    = 0x02,
    CS_BRAKE    = 0x04
};

enum _particleType {
    PT_SMOKE,
    PT_SPARK,
    PT_DIRT
};

b2Color carColor =          b2Color(1.0, 0.0, 0.0);
b2Color wheelColor =        b2Color(0.5, 0.5, 0.5);
b2Color concreteColor =     b2Color(0.9, 0.9, 0.9);
b2Color dirtColor =         b2Color(1.0, 0.6, 0.4);
b2Color sparkColor =        b2Color(0.9, 1.0, 0.1);


// bare-bones particle, using Box2D to update movement
// and get interaction with world objects. Life is used
// to fade out the particle.
struct simpleParticle {
    b2Body* body;
    float life;
};



// this callback will find the closest static body that the ray hits
class FirstHitGroundRayCastCallback : public b2RayCastCallback
{
public:
    b2Vec2 m_point;
    b2Fixture* m_fixture;

    FirstHitGroundRayCastCallback()
    {
        m_fixture = NULL;
    }

    float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction)
    {
        //ignore non-static bodies
        if ( fixture->GetBody()->GetType() != b2_staticBody )
            return -1;

        m_point = point;
        m_fixture = fixture;
        return fraction;
    }
};



class iforce2d_PhysicsDrivenParticles : public Test
{
public:
    iforce2d_PhysicsDrivenParticles()
	{		
        // the RUBE editor is used to set up the world, and the C++ source dump
        // feature makes a file we can include here without needing any JSON parser
        // http://www.iforce2d.net/rube
        #include "physicsDrivenParticles.cpp"

        m_carBody = NULL;
        m_frontWheelJoint = NULL;
        m_rearWheelJoint = NULL;

        // we can make use of knowledge about the scene to find items of interest
        // just by looking at their body type, position, mass etc. This is a hacky
        // method, not suitable for large scenes, but works ok for this small demo
        // Here we find the car body, wheel joints, ground fixtures, etc:
        for (b2Body* b = m_world->GetBodyList(); b; b = b->GetNext()) {
            if ( b->GetType() == b2_dynamicBody ) {
                //dynamic body with mass > 3 must be car body
                if ( b->GetMass() > 3 ) {
                    m_carBody = b;
                    m_carFixture = m_carBody->GetFixtureList();
                }
            }
            else if ( b->GetType() == b2_staticBody ) {
                //all fixtures on a static body are ground fixtures
                for (b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext()) {
                    m_groundFixtures.push_back(f);
                }
            }
        }
        for (b2Joint* j = m_world->GetJointList(); j; j = j->GetNext()) {
            if ( j->GetType() == e_wheelJoint ) {
                //we know there are only 2 joints in the scene
                if ( m_frontWheelJoint == NULL )
                    m_frontWheelJoint = (b2WheelJoint*)j;
                else
                    m_rearWheelJoint = (b2WheelJoint*)j;
            }
        }

        if ( m_frontWheelJoint && m_rearWheelJoint ) {
            //make sure the front and rear joint are the right way around
            if ( m_frontWheelJoint->GetBodyB()->GetPosition().x < m_rearWheelJoint->GetBodyB()->GetPosition().x )
                b2Swap( m_frontWheelJoint, m_rearWheelJoint );
            m_wheelFixtures.push_back(m_frontWheelJoint->GetBodyB()->GetFixtureList());
            m_wheelFixtures.push_back(m_rearWheelJoint->GetBodyB()->GetFixtureList());
        }

        //initial state
        m_controlState = 0;
        m_4wd = 0;

        //next index for particle generation
        m_nextParticleIndex = 0;
    }



    void Keyboard(unsigned char key)
    {
        switch (key)
        {
        case 'a': m_controlState |= CS_LEFT; break;
        case 'd': m_controlState |= CS_RIGHT; break;
        case 's': m_controlState |= CS_BRAKE; break;
        case '4': m_4wd = !m_4wd; break;
        case 'f': g_followCar = !g_followCar; break;
        }
    }

    void KeyboardUp(unsigned char key)
    {
        switch (key)
        {
        case 'a': m_controlState &= ~CS_LEFT; break;
        case 'd': m_controlState &= ~CS_RIGHT; break;
        case 's': m_controlState &= ~CS_BRAKE; break;
        }
    }



    // functions to determine if a fixture is a certain material type
    bool fixtureIsSteel(b2Fixture* f) { return f == m_carFixture; }
    bool fixtureIsConcrete(b2Fixture* f) { return f->GetBody()->GetType() == b2_staticBody && f->GetFriction() > 0.5; }
    bool fixtureIsDirt(b2Fixture* f) { return f->GetBody()->GetType() == b2_staticBody && f->GetFriction() < 0.5; }
    bool fixtureIsRubber(b2Fixture* f) { return f != m_carFixture && f->GetBody()->GetType() == b2_dynamicBody && f->GetBody()->GetMass() > 0.4; }

    /*
    The CHECK_MAT_VS_MAT macro below will make functions like this:

    bool contactIsSteelVsConcrete(b2Contact* contact) {
        b2Fixture* fA = contact->GetFixtureA();
        b2Fixture* fB = contact->GetFixtureB();
        if ( fixtureIsSteel(fA) && fixtureIsConcrete(fB) )
            return true;
        if ( fixtureIsSteel(fB) && fixtureIsConcrete(fA) )
            return true;
        return false;
    }*/

#define CHECK_MAT_VS_MAT(mat1, mat2)\
    bool contactIs##mat1##Vs##mat2(b2Contact* contact) {\
        b2Fixture* fA = contact->GetFixtureA();\
        b2Fixture* fB = contact->GetFixtureB();\
        if ( fixtureIs##mat1(fA) && fixtureIs##mat2(fB) )\
            return true;\
        if ( fixtureIs##mat1(fB) && fixtureIs##mat2(fA) )\
            return true;\
        return false;\
    }

    CHECK_MAT_VS_MAT(Steel, Concrete)
    CHECK_MAT_VS_MAT(Steel, Dirt)
    CHECK_MAT_VS_MAT(Rubber, Concrete)
    CHECK_MAT_VS_MAT(Rubber, Dirt)

    void BeginContact(b2Contact* contact)
    {
        if ( contactIsSteelVsConcrete(contact) )  m_steelToConcreteContacts.insert(contact);
        if ( contactIsRubberVsConcrete(contact) ) m_rubberToConcreteContacts.insert(contact);
        if ( contactIsSteelVsDirt(contact) )      m_steelToDirtContacts.insert(contact);
        if ( contactIsRubberVsDirt(contact) )     m_rubberToDirtContacts.insert(contact);
    }

    void EndContact(b2Contact* contact)
    {
        if ( contactIsSteelVsConcrete(contact) )  m_steelToConcreteContacts.erase(contact);
        if ( contactIsRubberVsConcrete(contact) ) m_rubberToConcreteContacts.erase(contact);
        if ( contactIsSteelVsDirt(contact) )      m_steelToDirtContacts.erase(contact);
        if ( contactIsRubberVsDirt(contact) )     m_rubberToDirtContacts.erase(contact);
    }


    // Here is where we decide whether a particle should be generated for the
    // currently active contacts. This function takes a pointer to a set, and
    // the particle type for that set, and uses the same logic for any set.
    // You could also pass the threshold as a parameter, for different criteria.
    void checkParticleGeneratingContacts(set<b2Contact*>* whichSet, _particleType whichParticleType)
    {
        float threshold = 1;
        for (set<b2Contact*>::iterator it = whichSet->begin(); it != whichSet->end(); ++it) {
            b2Contact* contact = *it;
            if ( contact->GetManifold()->pointCount < 1 )
                continue;
            b2Fixture* fA = contact->GetFixtureA();
            b2Fixture* fB = contact->GetFixtureB();
            b2Body* bA = fA->GetBody();
            b2Body* bB = fB->GetBody();
            b2WorldManifold worldManifold;
            contact->GetWorldManifold( &worldManifold );
            b2Vec2 worldPoint = worldManifold.points[0];
            b2Vec2 velA = bA->GetLinearVelocityFromWorldPoint(worldPoint);
            b2Vec2 velB = bB->GetLinearVelocityFromWorldPoint(worldPoint);
            float relativeSpeed = (velA - velB).Length();
            float totalFriction = fA->GetFriction() * fB->GetFriction();
            float intensity = relativeSpeed * totalFriction;
            if ( intensity > threshold )
                spawnParticle(whichParticleType, worldPoint, velA, velB, intensity );
        }
    }

    // Creates a bare-bones particle and returns it. Note that the
    // category bits for particles is set to 2, and the mask bits is
    // set to 1, so that particles will not collide with other particles.
    // The car and tire fixtures also have bit 2 removed from their mask
    // bits, so the car and tires will not collide with particles either.
    simpleParticle* createParticle()
    {
        simpleParticle* p = new simpleParticle;
        p->life = 1;

        b2BodyDef bd;
        bd.type = b2_dynamicBody;
        bd.fixedRotation = true;
        p->body = m_world->CreateBody(&bd);

        b2CircleShape circleShape;
        circleShape.m_radius = 0.01;

        b2FixtureDef fd;
        fd.density = 1;
        fd.filter.categoryBits = 2;
        fd.filter.maskBits = 1;
        fd.shape = &circleShape;
        p->body->CreateFixture(&fd);

        return p;
    }

    // use a raycast to find the height of the ground
    float heightOfGroundAt(float x)
    {
        b2Vec2 rayStart( x, 1000 );
        b2Vec2 rayEnd(x, -1000);
        FirstHitGroundRayCastCallback raycastCallback;
        m_world->RayCast(&raycastCallback, rayStart, rayEnd);
        if ( !raycastCallback.m_fixture )
            return -1000;
        return raycastCallback.m_point.y;
    }

    // creates (or recycles) a particle and sets it up with the appropriate
    // start position, velocity, gravity scale, restitution etc.
    void spawnParticle(_particleType particleType, b2Vec2 worldPoint, b2Vec2 velA, b2Vec2 velB, float intensity)
    {
        // first, figure out if we should make a new particle, or recycle one
        int currentParticleIndex = m_nextParticleIndex;
        if ( m_particles.size() < MAX_PARTICLES ) {
            //create a new particle
            m_particles.push_back( createParticle() );
            m_nextParticleIndex++;
        }
        else {
            //recycle oldest existing particle
            currentParticleIndex %= MAX_PARTICLES;
            m_nextParticleIndex = (m_nextParticleIndex+1) % MAX_PARTICLES;
        }

        // get a reference to the particle to be (re)positioned
        simpleParticle* p = m_particles[ currentParticleIndex ];
        b2Body* b = p->body;
        b2Fixture* f = b->GetFixtureList();

        // set the starting life (in this example, this is used for opacity)
        p->life = intensity * 0.3;
        p->life = b2Clamp(p->life, 0.0f, 1.0f);

        // set the starting position
        b->SetTransform( b2Vec2(worldPoint.x, heightOfGroundAt(worldPoint.x) + 0.05), 0 );

        // Adding the velocities of the two fixtures gives an average movement.
        // Depending on what kind of particles are being generated, you might
        // want to bias the starting velocity toward one of the fixtures involved.
        b2Vec2 vel = velA + velB;

        // set the appropriate properties, and put the particle in the appropriate list
        if ( particleType == PT_SMOKE ) {
            vel *= 0.1;
            vel.x += RandomFloat(-2,2);
            vel.y += RandomFloat(-2,2);
            b->SetLinearVelocity( vel );
            b->SetGravityScale(0);
            b->SetLinearDamping(4);
            f->SetRestitution(0.1);
            f->SetFriction(2);

            m_smokeParticles.insert(p);
            m_sparkParticles.erase(p);
            m_dirtParticles.erase(p);
        }
        else if ( particleType == PT_SPARK ) {
            vel.x = RandomFloat(-3,3);
            vel.y = RandomFloat(-3,3);
            b->SetLinearVelocity( vel );
            b->SetGravityScale(0.5);
            b->SetLinearDamping(1);
            f->SetRestitution(0.8);
            f->SetFriction(2);

            m_smokeParticles.erase(p);
            m_sparkParticles.insert(p);
            m_dirtParticles.erase(p);
        }
        else if ( particleType == PT_DIRT ) {
            vel *= 0.3;
            vel.x += RandomFloat(-2,2);
            vel.y += RandomFloat(-2,2);
            b->SetLinearVelocity( vel );
            b->SetGravityScale(1);
            b->SetLinearDamping(0.1);
            f->SetRestitution(0.2);
            f->SetFriction(2);

            m_smokeParticles.erase(p);
            m_sparkParticles.erase(p);
            m_dirtParticles.insert(p);
        }
    }

    // draw a simple point for each particle in the given set
    void drawParticles(set<simpleParticle*>* whichSet, b2Color whichColor)
    {
        glPointSize(8);
        glBegin(GL_POINTS);
        for (set<simpleParticle*>::iterator it = whichSet->begin(); it != whichSet->end(); ++it) {
            simpleParticle* p = *it;
            if ( p->life <= 0 )
                continue;
            b2Vec2 pos = p->body->GetPosition();
            glColor4f(whichColor.r, whichColor.g, whichColor.b, p->life);
            glVertex2f(pos.x, pos.y);
        }
        glEnd();
    }

    // decrease the life (opacity for this example) of each particle in the given set
    void decreaseParticleLife(set<simpleParticle*>* whichSet, float howMuch)
    {
        for (set<simpleParticle*>::iterator it = whichSet->begin(); it != whichSet->end(); ++it) {
            simpleParticle* p = *it;
            p->life -= howMuch;
        }
    }



	void Step(Settings* settings)
    {
        // update wheel joint motors from user input
        if ( m_controlState == 0 ) {
            // free roll...
            m_frontWheelJoint->EnableMotor(false);
            m_rearWheelJoint->EnableMotor(false);
        }
        else {
            m_rearWheelJoint->EnableMotor(true);

            if ( m_controlState & CS_BRAKE ) {
                m_frontWheelJoint->EnableMotor(true);
                m_frontWheelJoint->SetMotorSpeed(0);
                m_rearWheelJoint->SetMotorSpeed(0);
            }
            else {
                m_frontWheelJoint->EnableMotor(m_4wd);

                float speed = 1600;
                if ( m_controlState == CS_LEFT ) {
                    m_rearWheelJoint->SetMotorSpeed(speed*DEGTORAD);
                    m_frontWheelJoint->SetMotorSpeed(speed*DEGTORAD*m_4wd);
                }
                else if ( m_controlState == CS_RIGHT ) {
                    m_rearWheelJoint->SetMotorSpeed(-speed*DEGTORAD);
                    m_frontWheelJoint->SetMotorSpeed(-speed*DEGTORAD*m_4wd);
                }
            }
        }

        // update particles if simulation is running
        if ( !settings->pause ) {
            decreaseParticleLife(&m_smokeParticles, 0.01);
            decreaseParticleLife(&m_sparkParticles, 0.015);
            decreaseParticleLife(&m_dirtParticles, 0.005);
            checkParticleGeneratingContacts(&m_steelToConcreteContacts, PT_SPARK);
            checkParticleGeneratingContacts(&m_rubberToConcreteContacts, PT_SMOKE);
            checkParticleGeneratingContacts(&m_steelToDirtContacts, PT_DIRT);
            checkParticleGeneratingContacts(&m_rubberToDirtContacts, PT_DIRT);
        }

        // move view center to car, if user wants to
        if ( g_followCar )
            settings->viewCenter = m_carBody->GetPosition();


        // temporarily set the debug draw of the world to NULL, so that the normal
        // debug draw is not rendered (because we want to use our own colors below)
        m_world->SetDebugDraw(NULL);
		Test::Step(settings);
        m_world->SetDebugDraw(&m_debugDraw);


        // draw the fixtures in the colors we really wanted
        // *** you will need to make the DrawShape function public in b2World ***
        // Alternatively, comment out this section, and the two lines above with SetDebugDraw,
        // to use the default colors for fixtures
        for (int i = 0; i < m_groundFixtures.size(); i++) {
            b2Fixture* f = m_groundFixtures[i];
            if ( f->GetFriction() > 0.5 )
                m_world->DrawShape(f, f->GetBody()->GetTransform(), concreteColor);
            else
                m_world->DrawShape(f, f->GetBody()->GetTransform(), dirtColor);
        }
        for (int i = 0; i < m_wheelFixtures.size(); i++) {
            b2Fixture* f = m_wheelFixtures[i];
            m_world->DrawShape(f, f->GetBody()->GetTransform(), wheelColor);
        }
        m_world->DrawShape(m_carFixture, m_carFixture->GetBody()->GetTransform(), carColor);


        //draw the particles
        glEnable(GL_BLEND);
        drawParticles(&m_smokeParticles, wheelColor);
        drawParticles(&m_sparkParticles, sparkColor);
        drawParticles(&m_dirtParticles, dirtColor);

        // show the user how to work it
        m_debugDraw.DrawString(5, m_textLine, "Keys: a/s/d = left/brake/right, 4 = toggle 4wd, f = toggle follow car");
        m_textLine += DRAW_STRING_NEW_LINE;
        m_debugDraw.DrawString(5, m_textLine, "4wd is %s", m_4wd ? "on" : "off");
        m_textLine += DRAW_STRING_NEW_LINE;
	}

	static Test* Create()
	{
        return new iforce2d_PhysicsDrivenParticles;
    }


    //
    // class member variables
    //

    b2Body* m_carBody;
    b2Fixture* m_carFixture;

    b2WheelJoint* m_frontWheelJoint;
    b2WheelJoint* m_rearWheelJoint;
    vector<b2Fixture*> m_wheelFixtures;

    vector<b2Fixture*> m_groundFixtures;

    int m_controlState;
    int m_4wd;

    //references to currently active contacts
    set<b2Contact*> m_steelToConcreteContacts;
    set<b2Contact*> m_rubberToConcreteContacts;
    set<b2Contact*> m_steelToDirtContacts;
    set<b2Contact*> m_rubberToDirtContacts;

    //list of all particles, regardless of type
    vector<simpleParticle*> m_particles;
    int m_nextParticleIndex;

    //references to particles in m_particles
    set<simpleParticle*> m_smokeParticles;
    set<simpleParticle*> m_sparkParticles;
    set<simpleParticle*> m_dirtParticles;
};

#endif
