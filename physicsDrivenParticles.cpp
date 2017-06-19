//Source code dump of Box2D scene: physicsDrivenParticles.rube
//
//  Created by R.U.B.E 1.4.0
//  Using Box2D version 2.3.0
//  Tue July 23 2013 02:16:11
//
//  This code is originally intended for use in the Box2D testbed,
//  but you can easily use it in other applications by providing
//  a b2World for use as the 'm_world' variable in the code below.

b2Vec2 g(0.000000000000000e+00f, -1.000000000000000e+01f);
m_world->SetGravity(g);
b2Body** bodies = (b2Body**)b2Alloc(5 * sizeof(b2Body*));
b2Joint** joints = (b2Joint**)b2Alloc(2 * sizeof(b2Joint*));
{
  b2BodyDef bd;
  bd.type = b2BodyType(0);
  bd.position.Set(2.678408050537109e+01f, -2.058094406127930e+01f);
  bd.angle = 0.000000000000000e+00f;
  bd.linearVelocity.Set(0.000000000000000e+00f, 0.000000000000000e+00f);
  bd.angularVelocity = 0.000000000000000e+00f;
  bd.linearDamping = 0.000000000000000e+00f;
  bd.angularDamping = 0.000000000000000e+00f;
  bd.allowSleep = bool(4);
  bd.awake = bool(2);
  bd.fixedRotation = bool(0);
  bd.bullet = bool(0);
  bd.active = bool(32);
  bd.gravityScale = 1.000000000000000e+00f;
  bodies[0] = m_world->CreateBody(&bd);

  {
    b2FixtureDef fd;
    fd.friction = 2.500000000000000e-01f;
    fd.restitution = 0.000000000000000e+00f;
    fd.density = 1.000000000000000e+00f;
    fd.isSensor = bool(0);
    fd.filter.categoryBits = uint16(1);
    fd.filter.maskBits = uint16(65535);
    fd.filter.groupIndex = int16(0);
    b2PolygonShape shape;
    b2Vec2 vs[8];
    vs[0].Set(5.811504364013672e+00f, 2.047296142578125e+01f);
    vs[1].Set(4.032586097717285e+00f, 2.050083160400391e+01f);
    vs[2].Set(-5.177555084228516e-01f, 2.050087165832520e+01f);
    vs[3].Set(-5.177555084228516e-01f, 1.436721229553223e+01f);
    shape.Set(vs, 4);

    fd.shape = &shape;

    bodies[0]->CreateFixture(&fd);
  }
  {
    b2FixtureDef fd;
    fd.friction = 2.500000000000000e-01f;
    fd.restitution = 0.000000000000000e+00f;
    fd.density = 1.000000000000000e+00f;
    fd.isSensor = bool(0);
    fd.filter.categoryBits = uint16(1);
    fd.filter.maskBits = uint16(65535);
    fd.filter.groupIndex = int16(0);
    b2PolygonShape shape;
    b2Vec2 vs[8];
    vs[0].Set(5.811504364013672e+00f, 2.047296142578125e+01f);
    vs[1].Set(4.832534790039062e+00f, 2.058040809631348e+01f);
    vs[2].Set(4.032586097717285e+00f, 2.050083160400391e+01f);
    shape.Set(vs, 3);

    fd.shape = &shape;

    bodies[0]->CreateFixture(&fd);
  }
  {
    b2FixtureDef fd;
    fd.friction = 2.500000000000000e-01f;
    fd.restitution = 0.000000000000000e+00f;
    fd.density = 1.000000000000000e+00f;
    fd.isSensor = bool(0);
    fd.filter.categoryBits = uint16(1);
    fd.filter.maskBits = uint16(65535);
    fd.filter.groupIndex = int16(0);
    b2PolygonShape shape;
    b2Vec2 vs[8];
    vs[0].Set(6.635272979736328e+00f, 2.049683761596680e+01f);
    vs[1].Set(6.002525329589844e+00f, 2.056847000122070e+01f);
    vs[2].Set(5.811504364013672e+00f, 2.047296142578125e+01f);
    vs[3].Set(-5.177555084228516e-01f, 1.436721229553223e+01f);
    shape.Set(vs, 4);

    fd.shape = &shape;

    bodies[0]->CreateFixture(&fd);
  }
  {
    b2FixtureDef fd;
    fd.friction = 2.500000000000000e-01f;
    fd.restitution = 0.000000000000000e+00f;
    fd.density = 1.000000000000000e+00f;
    fd.isSensor = bool(0);
    fd.filter.categoryBits = uint16(1);
    fd.filter.maskBits = uint16(65535);
    fd.filter.groupIndex = int16(0);
    b2PolygonShape shape;
    b2Vec2 vs[8];
    vs[0].Set(9.166271209716797e+00f, 2.041326713562012e+01f);
    vs[1].Set(7.900772094726562e+00f, 2.065204048156738e+01f);
    vs[2].Set(6.635272979736328e+00f, 2.049683761596680e+01f);
    vs[3].Set(-5.177555084228516e-01f, 1.436721229553223e+01f);
    shape.Set(vs, 4);

    fd.shape = &shape;

    bodies[0]->CreateFixture(&fd);
  }
  {
    b2FixtureDef fd;
    fd.friction = 2.500000000000000e-01f;
    fd.restitution = 0.000000000000000e+00f;
    fd.density = 1.000000000000000e+00f;
    fd.isSensor = bool(0);
    fd.filter.categoryBits = uint16(1);
    fd.filter.maskBits = uint16(65535);
    fd.filter.groupIndex = int16(0);
    b2PolygonShape shape;
    b2Vec2 vs[8];
    vs[0].Set(1.156594085693359e+01f, 2.047296142578125e+01f);
    vs[1].Set(1.032432174682617e+01f, 2.054459190368652e+01f);
    vs[2].Set(9.166271209716797e+00f, 2.041326713562012e+01f);
    vs[3].Set(-5.177555084228516e-01f, 1.436721229553223e+01f);
    shape.Set(vs, 4);

    fd.shape = &shape;

    bodies[0]->CreateFixture(&fd);
  }
  {
    b2FixtureDef fd;
    fd.friction = 2.500000000000000e-01f;
    fd.restitution = 0.000000000000000e+00f;
    fd.density = 1.000000000000000e+00f;
    fd.isSensor = bool(0);
    fd.filter.categoryBits = uint16(1);
    fd.filter.maskBits = uint16(65535);
    fd.filter.groupIndex = int16(0);
    b2PolygonShape shape;
    b2Vec2 vs[8];
    vs[0].Set(1.259266662597656e+01f, 2.048489952087402e+01f);
    vs[1].Set(1.182859039306641e+01f, 2.052071571350098e+01f);
    vs[2].Set(1.156594085693359e+01f, 2.047296142578125e+01f);
    vs[3].Set(-5.177555084228516e-01f, 1.436721229553223e+01f);
    shape.Set(vs, 4);

    fd.shape = &shape;

    bodies[0]->CreateFixture(&fd);
  }
  {
    b2FixtureDef fd;
    fd.friction = 2.500000000000000e-01f;
    fd.restitution = 0.000000000000000e+00f;
    fd.density = 1.000000000000000e+00f;
    fd.isSensor = bool(0);
    fd.filter.categoryBits = uint16(1);
    fd.filter.maskBits = uint16(65535);
    fd.filter.groupIndex = int16(0);
    b2PolygonShape shape;
    b2Vec2 vs[8];
    vs[0].Set(3.169073867797852e+01f, 1.436697864532471e+01f);
    vs[1].Set(1.632285690307617e+01f, 2.048110389709473e+01f);
    vs[2].Set(1.521273422241211e+01f, 2.068668174743652e+01f);
    vs[3].Set(1.401336669921875e+01f, 2.061622428894043e+01f);
    vs[4].Set(1.259266662597656e+01f, 2.048489952087402e+01f);
    vs[5].Set(-5.177555084228516e-01f, 1.436721229553223e+01f);
    shape.Set(vs, 6);

    fd.shape = &shape;

    bodies[0]->CreateFixture(&fd);
  }
  {
    b2FixtureDef fd;
    fd.friction = 2.500000000000000e-01f;
    fd.restitution = 0.000000000000000e+00f;
    fd.density = 1.000000000000000e+00f;
    fd.isSensor = bool(0);
    fd.filter.categoryBits = uint16(1);
    fd.filter.maskBits = uint16(65535);
    fd.filter.groupIndex = int16(0);
    b2PolygonShape shape;
    b2Vec2 vs[8];
    vs[0].Set(3.169073867797852e+01f, 1.436697864532471e+01f);
    vs[1].Set(1.842214965820312e+01f, 2.043566703796387e+01f);
    vs[2].Set(1.707688522338867e+01f, 2.051671791076660e+01f);
    vs[3].Set(1.632285690307617e+01f, 2.048110389709473e+01f);
    shape.Set(vs, 4);

    fd.shape = &shape;

    bodies[0]->CreateFixture(&fd);
  }
  {
    b2FixtureDef fd;
    fd.friction = 2.500000000000000e-01f;
    fd.restitution = 0.000000000000000e+00f;
    fd.density = 1.000000000000000e+00f;
    fd.isSensor = bool(0);
    fd.filter.categoryBits = uint16(1);
    fd.filter.maskBits = uint16(65535);
    fd.filter.groupIndex = int16(0);
    b2PolygonShape shape;
    b2Vec2 vs[8];
    vs[0].Set(3.169073867797852e+01f, 1.436697864532471e+01f);
    vs[1].Set(2.014574432373047e+01f, 2.049510192871094e+01f);
    vs[2].Set(1.959102630615234e+01f, 2.057434844970703e+01f);
    vs[3].Set(1.842214965820312e+01f, 2.043566703796387e+01f);
    shape.Set(vs, 4);

    fd.shape = &shape;

    bodies[0]->CreateFixture(&fd);
  }
  {
    b2FixtureDef fd;
    fd.friction = 2.500000000000000e-01f;
    fd.restitution = 0.000000000000000e+00f;
    fd.density = 1.000000000000000e+00f;
    fd.isSensor = bool(0);
    fd.filter.categoryBits = uint16(1);
    fd.filter.maskBits = uint16(65535);
    fd.filter.groupIndex = int16(0);
    b2PolygonShape shape;
    b2Vec2 vs[8];
    vs[0].Set(3.169073867797852e+01f, 1.436697864532471e+01f);
    vs[1].Set(2.573258209228516e+01f, 2.073283958435059e+01f);
    vs[2].Set(2.470238494873047e+01f, 2.077246284484863e+01f);
    vs[3].Set(2.202783584594727e+01f, 2.077246284484863e+01f);
    vs[4].Set(2.014574432373047e+01f, 2.049510192871094e+01f);
    shape.Set(vs, 5);

    fd.shape = &shape;

    bodies[0]->CreateFixture(&fd);
  }
  {
    b2FixtureDef fd;
    fd.friction = 2.500000000000000e-01f;
    fd.restitution = 0.000000000000000e+00f;
    fd.density = 1.000000000000000e+00f;
    fd.isSensor = bool(0);
    fd.filter.categoryBits = uint16(1);
    fd.filter.maskBits = uint16(65535);
    fd.filter.groupIndex = int16(0);
    b2PolygonShape shape;
    b2Vec2 vs[8];
    vs[0].Set(2.202783584594727e+01f, 2.077246284484863e+01f);
    vs[1].Set(2.121556854248047e+01f, 2.091114234924316e+01f);
    vs[2].Set(2.014574432373047e+01f, 2.049510192871094e+01f);
    shape.Set(vs, 3);

    fd.shape = &shape;

    bodies[0]->CreateFixture(&fd);
  }
  {
    b2FixtureDef fd;
    fd.friction = 2.500000000000000e-01f;
    fd.restitution = 0.000000000000000e+00f;
    fd.density = 1.000000000000000e+00f;
    fd.isSensor = bool(0);
    fd.filter.categoryBits = uint16(1);
    fd.filter.maskBits = uint16(65535);
    fd.filter.groupIndex = int16(0);
    b2PolygonShape shape;
    b2Vec2 vs[8];
    vs[0].Set(2.470238494873047e+01f, 2.077246284484863e+01f);
    vs[1].Set(2.327595901489258e+01f, 2.097057723999023e+01f);
    vs[2].Set(2.202783584594727e+01f, 2.077246284484863e+01f);
    shape.Set(vs, 3);

    fd.shape = &shape;

    bodies[0]->CreateFixture(&fd);
  }
  {
    b2FixtureDef fd;
    fd.friction = 2.500000000000000e-01f;
    fd.restitution = 0.000000000000000e+00f;
    fd.density = 1.000000000000000e+00f;
    fd.isSensor = bool(0);
    fd.filter.categoryBits = uint16(1);
    fd.filter.maskBits = uint16(65535);
    fd.filter.groupIndex = int16(0);
    b2PolygonShape shape;
    b2Vec2 vs[8];
    vs[0].Set(3.169073867797852e+01f, 1.436697864532471e+01f);
    vs[1].Set(3.024960327148438e+01f, 2.077246284484863e+01f);
    vs[2].Set(2.919959259033203e+01f, 2.093095397949219e+01f);
    vs[3].Set(2.836751174926758e+01f, 2.097057723999023e+01f);
    vs[4].Set(2.731749725341797e+01f, 2.089133071899414e+01f);
    vs[5].Set(2.573258209228516e+01f, 2.073283958435059e+01f);
    shape.Set(vs, 6);

    fd.shape = &shape;

    bodies[0]->CreateFixture(&fd);
  }
  {
    b2FixtureDef fd;
    fd.friction = 2.500000000000000e-01f;
    fd.restitution = 0.000000000000000e+00f;
    fd.density = 1.000000000000000e+00f;
    fd.isSensor = bool(0);
    fd.filter.categoryBits = uint16(1);
    fd.filter.maskBits = uint16(65535);
    fd.filter.groupIndex = int16(0);
    b2PolygonShape shape;
    b2Vec2 vs[8];
    vs[0].Set(2.731749725341797e+01f, 2.089133071899414e+01f);
    vs[1].Set(2.646560668945312e+01f, 2.106963539123535e+01f);
    vs[2].Set(2.573258209228516e+01f, 2.073283958435059e+01f);
    shape.Set(vs, 3);

    fd.shape = &shape;

    bodies[0]->CreateFixture(&fd);
  }
  {
    b2FixtureDef fd;
    fd.friction = 2.500000000000000e-01f;
    fd.restitution = 0.000000000000000e+00f;
    fd.density = 1.000000000000000e+00f;
    fd.isSensor = bool(0);
    fd.filter.categoryBits = uint16(1);
    fd.filter.maskBits = uint16(65535);
    fd.filter.groupIndex = int16(0);
    b2PolygonShape shape;
    b2Vec2 vs[8];
    vs[0].Set(3.502416610717773e+01f, 2.059416007995605e+01f);
    vs[1].Set(3.371660995483398e+01f, 2.083189773559570e+01f);
    vs[2].Set(3.276565933227539e+01f, 2.087152099609375e+01f);
    vs[3].Set(3.118074035644531e+01f, 2.093095397949219e+01f);
    vs[4].Set(3.024960327148438e+01f, 2.077246284484863e+01f);
    vs[5].Set(3.169073867797852e+01f, 1.436697864532471e+01f);
    shape.Set(vs, 6);

    fd.shape = &shape;

    bodies[0]->CreateFixture(&fd);
  }
  {
    b2FixtureDef fd;
    fd.friction = 2.500000000000000e-01f;
    fd.restitution = 0.000000000000000e+00f;
    fd.density = 1.000000000000000e+00f;
    fd.isSensor = bool(0);
    fd.filter.categoryBits = uint16(1);
    fd.filter.maskBits = uint16(65535);
    fd.filter.groupIndex = int16(0);
    b2PolygonShape shape;
    b2Vec2 vs[8];
    vs[0].Set(3.276565933227539e+01f, 2.087152099609375e+01f);
    vs[1].Set(3.181470870971680e+01f, 2.104982376098633e+01f);
    vs[2].Set(3.118074035644531e+01f, 2.093095397949219e+01f);
    shape.Set(vs, 3);

    fd.shape = &shape;

    bodies[0]->CreateFixture(&fd);
  }
  {
    b2FixtureDef fd;
    fd.friction = 2.500000000000000e-01f;
    fd.restitution = 0.000000000000000e+00f;
    fd.density = 1.000000000000000e+00f;
    fd.isSensor = bool(0);
    fd.filter.categoryBits = uint16(1);
    fd.filter.maskBits = uint16(65535);
    fd.filter.groupIndex = int16(0);
    b2PolygonShape shape;
    b2Vec2 vs[8];
    vs[0].Set(3.502416610717773e+01f, 2.059416007995605e+01f);
    vs[1].Set(3.427133178710938e+01f, 2.095076560974121e+01f);
    vs[2].Set(3.371660995483398e+01f, 2.083189773559570e+01f);
    shape.Set(vs, 3);

    fd.shape = &shape;

    bodies[0]->CreateFixture(&fd);
  }
  {
    b2FixtureDef fd;
    fd.friction = 2.500000000000000e-01f;
    fd.restitution = 0.000000000000000e+00f;
    fd.density = 1.000000000000000e+00f;
    fd.isSensor = bool(0);
    fd.filter.categoryBits = uint16(1);
    fd.filter.maskBits = uint16(65535);
    fd.filter.groupIndex = int16(0);
    b2PolygonShape shape;
    b2Vec2 vs[8];
    vs[0].Set(5.931943511962891e+01f, 1.436677742004395e+01f);
    vs[1].Set(3.713806152343750e+01f, 2.066181373596191e+01f);
    vs[2].Set(3.571756744384766e+01f, 2.081208610534668e+01f);
    vs[3].Set(3.502416610717773e+01f, 2.059416007995605e+01f);
    vs[4].Set(3.169073867797852e+01f, 1.436697864532471e+01f);
    shape.Set(vs, 5);

    fd.shape = &shape;

    bodies[0]->CreateFixture(&fd);
  }
  {
    b2FixtureDef fd;
    fd.friction = 2.500000000000000e-01f;
    fd.restitution = 0.000000000000000e+00f;
    fd.density = 1.000000000000000e+00f;
    fd.isSensor = bool(0);
    fd.filter.categoryBits = uint16(1);
    fd.filter.maskBits = uint16(65535);
    fd.filter.groupIndex = int16(0);
    b2PolygonShape shape;
    b2Vec2 vs[8];
    vs[0].Set(5.931943511962891e+01f, 1.436677742004395e+01f);
    vs[1].Set(4.050456237792969e+01f, 2.066181373596191e+01f);
    vs[2].Set(3.890146636962891e+01f, 2.088624763488770e+01f);
    vs[3].Set(3.713806152343750e+01f, 2.066181373596191e+01f);
    shape.Set(vs, 4);

    fd.shape = &shape;

    bodies[0]->CreateFixture(&fd);
  }
  {
    b2FixtureDef fd;
    fd.friction = 2.500000000000000e-01f;
    fd.restitution = 0.000000000000000e+00f;
    fd.density = 1.000000000000000e+00f;
    fd.isSensor = bool(0);
    fd.filter.categoryBits = uint16(1);
    fd.filter.maskBits = uint16(65535);
    fd.filter.groupIndex = int16(0);
    b2PolygonShape shape;
    b2Vec2 vs[8];
    vs[0].Set(5.931943511962891e+01f, 1.436677742004395e+01f);
    vs[1].Set(4.829560852050781e+01f, 2.056562805175781e+01f);
    vs[2].Set(4.573065185546875e+01f, 2.075799942016602e+01f);
    vs[3].Set(4.367868804931641e+01f, 2.072593688964844e+01f);
    vs[4].Set(4.050456237792969e+01f, 2.066181373596191e+01f);
    shape.Set(vs, 5);

    fd.shape = &shape;

    bodies[0]->CreateFixture(&fd);
  }
  {
    b2FixtureDef fd;
    fd.friction = 2.500000000000000e-01f;
    fd.restitution = 0.000000000000000e+00f;
    fd.density = 1.000000000000000e+00f;
    fd.isSensor = bool(0);
    fd.filter.categoryBits = uint16(1);
    fd.filter.maskBits = uint16(65535);
    fd.filter.groupIndex = int16(0);
    b2PolygonShape shape;
    b2Vec2 vs[8];
    vs[0].Set(4.367868804931641e+01f, 2.072593688964844e+01f);
    vs[1].Set(4.210765838623047e+01f, 2.088624763488770e+01f);
    vs[2].Set(4.050456237792969e+01f, 2.066181373596191e+01f);
    shape.Set(vs, 3);

    fd.shape = &shape;

    bodies[0]->CreateFixture(&fd);
  }
  {
    b2FixtureDef fd;
    fd.friction = 2.500000000000000e-01f;
    fd.restitution = 0.000000000000000e+00f;
    fd.density = 1.000000000000000e+00f;
    fd.isSensor = bool(0);
    fd.filter.categoryBits = uint16(1);
    fd.filter.maskBits = uint16(65535);
    fd.filter.groupIndex = int16(0);
    b2PolygonShape shape;
    b2Vec2 vs[8];
    vs[0].Set(4.829560852050781e+01f, 2.056562805175781e+01f);
    vs[1].Set(4.678869628906250e+01f, 2.091830825805664e+01f);
    vs[2].Set(4.573065185546875e+01f, 2.075799942016602e+01f);
    shape.Set(vs, 3);

    fd.shape = &shape;

    bodies[0]->CreateFixture(&fd);
  }
  {
    b2FixtureDef fd;
    fd.friction = 2.500000000000000e-01f;
    fd.restitution = 0.000000000000000e+00f;
    fd.density = 1.000000000000000e+00f;
    fd.isSensor = bool(0);
    fd.filter.categoryBits = uint16(1);
    fd.filter.maskBits = uint16(65535);
    fd.filter.groupIndex = int16(0);
    b2PolygonShape shape;
    b2Vec2 vs[8];
    vs[0].Set(5.931943511962891e+01f, 1.436677742004395e+01f);
    vs[1].Set(5.627902984619141e+01f, 2.046944236755371e+01f);
    vs[2].Set(5.329726409912109e+01f, 2.069387626647949e+01f);
    vs[3].Set(5.179035949707031e+01f, 2.072593688964844e+01f);
    vs[4].Set(5.076437377929688e+01f, 2.072593688964844e+01f);
    vs[5].Set(4.829560852050781e+01f, 2.056562805175781e+01f);
    shape.Set(vs, 6);

    fd.shape = &shape;

    bodies[0]->CreateFixture(&fd);
  }
  {
    b2FixtureDef fd;
    fd.friction = 2.500000000000000e-01f;
    fd.restitution = 0.000000000000000e+00f;
    fd.density = 1.000000000000000e+00f;
    fd.isSensor = bool(0);
    fd.filter.categoryBits = uint16(1);
    fd.filter.maskBits = uint16(65535);
    fd.filter.groupIndex = int16(0);
    b2PolygonShape shape;
    b2Vec2 vs[8];
    vs[0].Set(5.076437377929688e+01f, 2.072593688964844e+01f);
    vs[1].Set(4.951396179199219e+01f, 2.082212257385254e+01f);
    vs[2].Set(4.829560852050781e+01f, 2.056562805175781e+01f);
    shape.Set(vs, 3);

    fd.shape = &shape;

    bodies[0]->CreateFixture(&fd);
  }
  {
    b2FixtureDef fd;
    fd.friction = 2.500000000000000e-01f;
    fd.restitution = 0.000000000000000e+00f;
    fd.density = 1.000000000000000e+00f;
    fd.isSensor = bool(0);
    fd.filter.categoryBits = uint16(1);
    fd.filter.maskBits = uint16(65535);
    fd.filter.groupIndex = int16(0);
    b2PolygonShape shape;
    b2Vec2 vs[8];
    vs[0].Set(5.627902984619141e+01f, 2.046944236755371e+01f);
    vs[1].Set(5.422706604003906e+01f, 2.088624763488770e+01f);
    vs[2].Set(5.329726409912109e+01f, 2.069387626647949e+01f);
    shape.Set(vs, 3);

    fd.shape = &shape;

    bodies[0]->CreateFixture(&fd);
  }
  {
    b2FixtureDef fd;
    fd.friction = 2.500000000000000e-01f;
    fd.restitution = 0.000000000000000e+00f;
    fd.density = 1.000000000000000e+00f;
    fd.isSensor = bool(0);
    fd.filter.categoryBits = uint16(1);
    fd.filter.maskBits = uint16(65535);
    fd.filter.groupIndex = int16(0);
    b2PolygonShape shape;
    b2Vec2 vs[8];
    vs[0].Set(5.931943511962891e+01f, 1.436677742004395e+01f);
    vs[1].Set(5.931943511962891e+01f, 2.050043678283691e+01f);
    vs[2].Set(5.833099365234375e+01f, 2.056562805175781e+01f);
    vs[3].Set(5.627902984619141e+01f, 2.046944236755371e+01f);
    shape.Set(vs, 4);

    fd.shape = &shape;

    bodies[0]->CreateFixture(&fd);
  }
  {
    b2FixtureDef fd;
    fd.friction = 2.500000000000000e-01f;
    fd.restitution = 0.000000000000000e+00f;
    fd.density = 1.000000000000000e+00f;
    fd.isSensor = bool(0);
    fd.filter.categoryBits = uint16(1);
    fd.filter.maskBits = uint16(65535);
    fd.filter.groupIndex = int16(0);
    b2PolygonShape shape;
    b2Vec2 vs[8];
    vs[0].Set(5.833099365234375e+01f, 2.056562805175781e+01f);
    vs[1].Set(5.724088287353516e+01f, 2.072593688964844e+01f);
    vs[2].Set(5.627902984619141e+01f, 2.046944236755371e+01f);
    shape.Set(vs, 3);

    fd.shape = &shape;

    bodies[0]->CreateFixture(&fd);
  }
}
{
  b2BodyDef bd;
  bd.type = b2BodyType(0);
  bd.position.Set(-3.508758544921875e-02f, -2.057919883728027e+01f);
  bd.angle = 0.000000000000000e+00f;
  bd.linearVelocity.Set(0.000000000000000e+00f, 0.000000000000000e+00f);
  bd.angularVelocity = 0.000000000000000e+00f;
  bd.linearDamping = 0.000000000000000e+00f;
  bd.angularDamping = 0.000000000000000e+00f;
  bd.allowSleep = bool(4);
  bd.awake = bool(2);
  bd.fixedRotation = bool(0);
  bd.bullet = bool(0);
  bd.active = bool(32);
  bd.gravityScale = 1.000000000000000e+00f;
  bodies[1] = m_world->CreateBody(&bd);

  {
    b2FixtureDef fd;
    fd.friction = 8.000000119209290e-01f;
    fd.restitution = 0.000000000000000e+00f;
    fd.density = 1.000000000000000e+00f;
    fd.isSensor = bool(0);
    fd.filter.categoryBits = uint16(1);
    fd.filter.maskBits = uint16(65535);
    fd.filter.groupIndex = int16(0);
    b2PolygonShape shape;
    b2Vec2 vs[8];
    vs[0].Set(-4.865343093872070e+00f, 2.050000000000000e+01f);
    vs[1].Set(-5.775164413452148e+01f, 2.050000000000000e+01f);
    vs[2].Set(-5.865258026123047e+01f, 1.436634063720703e+01f);
    vs[3].Set(-6.320858001708984e+00f, 1.436634063720703e+01f);
    shape.Set(vs, 4);

    fd.shape = &shape;

    bodies[1]->CreateFixture(&fd);
  }
  {
    b2FixtureDef fd;
    fd.friction = 8.000000119209290e-01f;
    fd.restitution = 0.000000000000000e+00f;
    fd.density = 1.000000000000000e+00f;
    fd.isSensor = bool(0);
    fd.filter.categoryBits = uint16(1);
    fd.filter.maskBits = uint16(65535);
    fd.filter.groupIndex = int16(0);
    b2PolygonShape shape;
    b2Vec2 vs[8];
    vs[0].Set(-5.775164413452148e+01f, 2.050000000000000e+01f);
    vs[1].Set(-5.775164413452148e+01f, 2.849147033691406e+01f);
    vs[2].Set(-5.865258026123047e+01f, 2.849147033691406e+01f);
    vs[3].Set(-5.865258026123047e+01f, 1.436634063720703e+01f);
    shape.Set(vs, 4);

    fd.shape = &shape;

    bodies[1]->CreateFixture(&fd);
  }
  {
    b2FixtureDef fd;
    fd.friction = 8.000000119209290e-01f;
    fd.restitution = 0.000000000000000e+00f;
    fd.density = 1.000000000000000e+00f;
    fd.isSensor = bool(0);
    fd.filter.categoryBits = uint16(1);
    fd.filter.maskBits = uint16(65535);
    fd.filter.groupIndex = int16(0);
    b2PolygonShape shape;
    b2Vec2 vs[8];
    vs[0].Set(-4.865343093872070e+00f, 2.050000000000000e+01f);
    vs[1].Set(-5.439317703247070e+00f, 2.073865890502930e+01f);
    vs[2].Set(-7.352565765380859e+00f, 2.073865890502930e+01f);
    vs[3].Set(-7.981204986572266e+00f, 2.050000000000000e+01f);
    shape.Set(vs, 4);

    fd.shape = &shape;

    bodies[1]->CreateFixture(&fd);
  }
  {
    b2FixtureDef fd;
    fd.friction = 8.000000119209290e-01f;
    fd.restitution = 0.000000000000000e+00f;
    fd.density = 1.000000000000000e+00f;
    fd.isSensor = bool(0);
    fd.filter.categoryBits = uint16(1);
    fd.filter.maskBits = uint16(65535);
    fd.filter.groupIndex = int16(0);
    b2PolygonShape shape;
    b2Vec2 vs[8];
    vs[0].Set(1.134078407287598e+01f, 1.436634063720703e+01f);
    vs[1].Set(8.904533386230469e+00f, 2.050000000000000e+01f);
    vs[2].Set(-4.865343093872070e+00f, 2.050000000000000e+01f);
    vs[3].Set(-6.320858001708984e+00f, 1.436634063720703e+01f);
    shape.Set(vs, 4);

    fd.shape = &shape;

    bodies[1]->CreateFixture(&fd);
  }
  {
    b2FixtureDef fd;
    fd.friction = 8.000000119209290e-01f;
    fd.restitution = 0.000000000000000e+00f;
    fd.density = 1.000000000000000e+00f;
    fd.isSensor = bool(0);
    fd.filter.categoryBits = uint16(1);
    fd.filter.maskBits = uint16(65535);
    fd.filter.groupIndex = int16(0);
    b2PolygonShape shape;
    b2Vec2 vs[8];
    vs[0].Set(1.134986305236816e+01f, 2.057275009155273e+01f);
    vs[1].Set(1.098562240600586e+01f, 2.083854675292969e+01f);
    vs[2].Set(9.597747802734375e+00f, 2.083854675292969e+01f);
    vs[3].Set(8.904533386230469e+00f, 2.050000000000000e+01f);
    vs[4].Set(1.134078407287598e+01f, 1.436634063720703e+01f);
    shape.Set(vs, 5);

    fd.shape = &shape;

    bodies[1]->CreateFixture(&fd);
  }
  {
    b2FixtureDef fd;
    fd.friction = 8.000000119209290e-01f;
    fd.restitution = 0.000000000000000e+00f;
    fd.density = 1.000000000000000e+00f;
    fd.isSensor = bool(0);
    fd.filter.categoryBits = uint16(1);
    fd.filter.maskBits = uint16(65535);
    fd.filter.groupIndex = int16(0);
    b2PolygonShape shape;
    b2Vec2 vs[8];
    vs[0].Set(1.359582519531250e+01f, 2.050000000000000e+01f);
    vs[1].Set(1.291873168945312e+01f, 2.083854675292969e+01f);
    vs[2].Set(1.175840187072754e+01f, 2.083854675292969e+01f);
    vs[3].Set(1.134986305236816e+01f, 2.057275009155273e+01f);
    vs[4].Set(1.134078407287598e+01f, 1.436634063720703e+01f);
    shape.Set(vs, 5);

    fd.shape = &shape;

    bodies[1]->CreateFixture(&fd);
  }
  {
    b2FixtureDef fd;
    fd.friction = 8.000000119209290e-01f;
    fd.restitution = 0.000000000000000e+00f;
    fd.density = 1.000000000000000e+00f;
    fd.isSensor = bool(0);
    fd.filter.categoryBits = uint16(1);
    fd.filter.maskBits = uint16(65535);
    fd.filter.groupIndex = int16(0);
    b2PolygonShape shape;
    b2Vec2 vs[8];
    vs[0].Set(2.630141258239746e+01f, 1.436546897888184e+01f);
    vs[1].Set(2.630141258239746e+01f, 2.049912834167480e+01f);
    vs[2].Set(1.359582519531250e+01f, 2.050000000000000e+01f);
    vs[3].Set(1.134078407287598e+01f, 1.436634063720703e+01f);
    shape.Set(vs, 4);

    fd.shape = &shape;

    bodies[1]->CreateFixture(&fd);
  }
  {
    b2FixtureDef fd;
    fd.friction = 8.000000119209290e-01f;
    fd.restitution = 0.000000000000000e+00f;
    fd.density = 1.000000000000000e+00f;
    fd.isSensor = bool(0);
    fd.filter.categoryBits = uint16(1);
    fd.filter.maskBits = uint16(65535);
    fd.filter.groupIndex = int16(0);
    b2PolygonShape shape;
    b2Vec2 vs[8];
    vs[0].Set(1.010914611816406e+02f, 2.057275009155273e+01f);
    vs[1].Set(1.006829223632812e+02f, 2.083854675292969e+01f);
    vs[2].Set(9.952259063720703e+01f, 2.083854675292969e+01f);
    shape.Set(vs, 3);

    fd.shape = &shape;

    bodies[1]->CreateFixture(&fd);
  }
  {
    b2FixtureDef fd;
    fd.friction = 8.000000119209290e-01f;
    fd.restitution = 0.000000000000000e+00f;
    fd.density = 1.000000000000000e+00f;
    fd.isSensor = bool(0);
    fd.filter.categoryBits = uint16(1);
    fd.filter.maskBits = uint16(65535);
    fd.filter.groupIndex = int16(0);
    b2PolygonShape shape;
    b2Vec2 vs[8];
    vs[0].Set(1.010914611816406e+02f, 2.057275009155273e+01f);
    vs[1].Set(9.952259063720703e+01f, 2.083854675292969e+01f);
    vs[2].Set(9.884549713134766e+01f, 2.050000000000000e+01f);
    shape.Set(vs, 3);

    fd.shape = &shape;

    bodies[1]->CreateFixture(&fd);
  }
  {
    b2FixtureDef fd;
    fd.friction = 8.000000119209290e-01f;
    fd.restitution = 0.000000000000000e+00f;
    fd.density = 1.000000000000000e+00f;
    fd.isSensor = bool(0);
    fd.filter.categoryBits = uint16(1);
    fd.filter.maskBits = uint16(65535);
    fd.filter.groupIndex = int16(0);
    b2PolygonShape shape;
    b2Vec2 vs[8];
    vs[0].Set(1.028435745239258e+02f, 2.083854675292969e+01f);
    vs[1].Set(1.014556961059570e+02f, 2.083854675292969e+01f);
    vs[2].Set(1.010914611816406e+02f, 2.057275009155273e+01f);
    shape.Set(vs, 3);

    fd.shape = &shape;

    bodies[1]->CreateFixture(&fd);
  }
  {
    b2FixtureDef fd;
    fd.friction = 8.000000119209290e-01f;
    fd.restitution = 0.000000000000000e+00f;
    fd.density = 1.000000000000000e+00f;
    fd.isSensor = bool(0);
    fd.filter.categoryBits = uint16(1);
    fd.filter.maskBits = uint16(65535);
    fd.filter.groupIndex = int16(0);
    b2PolygonShape shape;
    b2Vec2 vs[8];
    vs[0].Set(1.035367889404297e+02f, 2.050000000000000e+01f);
    vs[1].Set(1.028435745239258e+02f, 2.083854675292969e+01f);
    vs[2].Set(1.010914611816406e+02f, 2.057275009155273e+01f);
    shape.Set(vs, 3);

    fd.shape = &shape;

    bodies[1]->CreateFixture(&fd);
  }
  {
    b2FixtureDef fd;
    fd.friction = 8.000000119209290e-01f;
    fd.restitution = 0.000000000000000e+00f;
    fd.density = 1.000000000000000e+00f;
    fd.isSensor = bool(0);
    fd.filter.categoryBits = uint16(1);
    fd.filter.maskBits = uint16(65535);
    fd.filter.groupIndex = int16(0);
    b2PolygonShape shape;
    b2Vec2 vs[8];
    vs[0].Set(1.197938919067383e+02f, 2.073865890502930e+01f);
    vs[1].Set(1.178806381225586e+02f, 2.073865890502930e+01f);
    vs[2].Set(1.173066635131836e+02f, 2.050000000000000e+01f);
    shape.Set(vs, 3);

    fd.shape = &shape;

    bodies[1]->CreateFixture(&fd);
  }
  {
    b2FixtureDef fd;
    fd.friction = 8.000000119209290e-01f;
    fd.restitution = 0.000000000000000e+00f;
    fd.density = 1.000000000000000e+00f;
    fd.isSensor = bool(0);
    fd.filter.categoryBits = uint16(1);
    fd.filter.maskBits = uint16(65535);
    fd.filter.groupIndex = int16(0);
    b2PolygonShape shape;
    b2Vec2 vs[8];
    vs[0].Set(1.270544509887695e+02f, 2.849147033691406e+01f);
    vs[1].Set(1.261535110473633e+02f, 2.849147033691406e+01f);
    vs[2].Set(1.261535110473633e+02f, 2.050000000000000e+01f);
    shape.Set(vs, 3);

    fd.shape = &shape;

    bodies[1]->CreateFixture(&fd);
  }
  {
    b2FixtureDef fd;
    fd.friction = 8.000000119209290e-01f;
    fd.restitution = 0.000000000000000e+00f;
    fd.density = 1.000000000000000e+00f;
    fd.isSensor = bool(0);
    fd.filter.categoryBits = uint16(1);
    fd.filter.maskBits = uint16(65535);
    fd.filter.groupIndex = int16(0);
    b2PolygonShape shape;
    b2Vec2 vs[8];
    vs[0].Set(1.270544509887695e+02f, 1.436634063720703e+01f);
    vs[1].Set(1.270544509887695e+02f, 2.849147033691406e+01f);
    vs[2].Set(1.261535110473633e+02f, 2.050000000000000e+01f);
    shape.Set(vs, 3);

    fd.shape = &shape;

    bodies[1]->CreateFixture(&fd);
  }
  {
    b2FixtureDef fd;
    fd.friction = 8.000000119209290e-01f;
    fd.restitution = 0.000000000000000e+00f;
    fd.density = 1.000000000000000e+00f;
    fd.isSensor = bool(0);
    fd.filter.categoryBits = uint16(1);
    fd.filter.maskBits = uint16(65535);
    fd.filter.groupIndex = int16(0);
    b2PolygonShape shape;
    b2Vec2 vs[8];
    vs[0].Set(1.270544509887695e+02f, 1.436634063720703e+01f);
    vs[1].Set(1.261535110473633e+02f, 2.050000000000000e+01f);
    vs[2].Set(1.204225234985352e+02f, 2.050000000000000e+01f);
    shape.Set(vs, 3);

    fd.shape = &shape;

    bodies[1]->CreateFixture(&fd);
  }
  {
    b2FixtureDef fd;
    fd.friction = 8.000000119209290e-01f;
    fd.restitution = 0.000000000000000e+00f;
    fd.density = 1.000000000000000e+00f;
    fd.isSensor = bool(0);
    fd.filter.categoryBits = uint16(1);
    fd.filter.maskBits = uint16(65535);
    fd.filter.groupIndex = int16(0);
    b2PolygonShape shape;
    b2Vec2 vs[8];
    vs[0].Set(1.270544509887695e+02f, 1.436634063720703e+01f);
    vs[1].Set(1.204225234985352e+02f, 2.050000000000000e+01f);
    vs[2].Set(1.187621841430664e+02f, 1.436634063720703e+01f);
    shape.Set(vs, 3);

    fd.shape = &shape;

    bodies[1]->CreateFixture(&fd);
  }
  {
    b2FixtureDef fd;
    fd.friction = 8.000000119209290e-01f;
    fd.restitution = 0.000000000000000e+00f;
    fd.density = 1.000000000000000e+00f;
    fd.isSensor = bool(0);
    fd.filter.categoryBits = uint16(1);
    fd.filter.maskBits = uint16(65535);
    fd.filter.groupIndex = int16(0);
    b2PolygonShape shape;
    b2Vec2 vs[8];
    vs[0].Set(1.204225234985352e+02f, 2.050000000000000e+01f);
    vs[1].Set(1.197938919067383e+02f, 2.073865890502930e+01f);
    vs[2].Set(1.187621841430664e+02f, 1.436634063720703e+01f);
    shape.Set(vs, 3);

    fd.shape = &shape;

    bodies[1]->CreateFixture(&fd);
  }
  {
    b2FixtureDef fd;
    fd.friction = 8.000000119209290e-01f;
    fd.restitution = 0.000000000000000e+00f;
    fd.density = 1.000000000000000e+00f;
    fd.isSensor = bool(0);
    fd.filter.categoryBits = uint16(1);
    fd.filter.maskBits = uint16(65535);
    fd.filter.groupIndex = int16(0);
    b2PolygonShape shape;
    b2Vec2 vs[8];
    vs[0].Set(1.197938919067383e+02f, 2.073865890502930e+01f);
    vs[1].Set(1.173066635131836e+02f, 2.050000000000000e+01f);
    vs[2].Set(1.187621841430664e+02f, 1.436634063720703e+01f);
    shape.Set(vs, 3);

    fd.shape = &shape;

    bodies[1]->CreateFixture(&fd);
  }
  {
    b2FixtureDef fd;
    fd.friction = 8.000000119209290e-01f;
    fd.restitution = 0.000000000000000e+00f;
    fd.density = 1.000000000000000e+00f;
    fd.isSensor = bool(0);
    fd.filter.categoryBits = uint16(1);
    fd.filter.maskBits = uint16(65535);
    fd.filter.groupIndex = int16(0);
    b2PolygonShape shape;
    b2Vec2 vs[8];
    vs[0].Set(1.187621841430664e+02f, 1.436634063720703e+01f);
    vs[1].Set(1.173066635131836e+02f, 2.050000000000000e+01f);
    vs[2].Set(1.035367889404297e+02f, 2.050000000000000e+01f);
    shape.Set(vs, 3);

    fd.shape = &shape;

    bodies[1]->CreateFixture(&fd);
  }
  {
    b2FixtureDef fd;
    fd.friction = 8.000000119209290e-01f;
    fd.restitution = 0.000000000000000e+00f;
    fd.density = 1.000000000000000e+00f;
    fd.isSensor = bool(0);
    fd.filter.categoryBits = uint16(1);
    fd.filter.maskBits = uint16(65535);
    fd.filter.groupIndex = int16(0);
    b2PolygonShape shape;
    b2Vec2 vs[8];
    vs[0].Set(1.187621841430664e+02f, 1.436634063720703e+01f);
    vs[1].Set(1.035367889404297e+02f, 2.050000000000000e+01f);
    vs[2].Set(1.011005401611328e+02f, 1.436634063720703e+01f);
    shape.Set(vs, 3);

    fd.shape = &shape;

    bodies[1]->CreateFixture(&fd);
  }
  {
    b2FixtureDef fd;
    fd.friction = 8.000000119209290e-01f;
    fd.restitution = 0.000000000000000e+00f;
    fd.density = 1.000000000000000e+00f;
    fd.isSensor = bool(0);
    fd.filter.categoryBits = uint16(1);
    fd.filter.maskBits = uint16(65535);
    fd.filter.groupIndex = int16(0);
    b2PolygonShape shape;
    b2Vec2 vs[8];
    vs[0].Set(1.035367889404297e+02f, 2.050000000000000e+01f);
    vs[1].Set(1.010914611816406e+02f, 2.057275009155273e+01f);
    vs[2].Set(1.011005401611328e+02f, 1.436634063720703e+01f);
    shape.Set(vs, 3);

    fd.shape = &shape;

    bodies[1]->CreateFixture(&fd);
  }
  {
    b2FixtureDef fd;
    fd.friction = 8.000000119209290e-01f;
    fd.restitution = 0.000000000000000e+00f;
    fd.density = 1.000000000000000e+00f;
    fd.isSensor = bool(0);
    fd.filter.categoryBits = uint16(1);
    fd.filter.maskBits = uint16(65535);
    fd.filter.groupIndex = int16(0);
    b2PolygonShape shape;
    b2Vec2 vs[8];
    vs[0].Set(1.011005401611328e+02f, 1.436634063720703e+01f);
    vs[1].Set(1.010914611816406e+02f, 2.057275009155273e+01f);
    vs[2].Set(9.884549713134766e+01f, 2.050000000000000e+01f);
    shape.Set(vs, 3);

    fd.shape = &shape;

    bodies[1]->CreateFixture(&fd);
  }
  {
    b2FixtureDef fd;
    fd.friction = 8.000000119209290e-01f;
    fd.restitution = 0.000000000000000e+00f;
    fd.density = 1.000000000000000e+00f;
    fd.isSensor = bool(0);
    fd.filter.categoryBits = uint16(1);
    fd.filter.maskBits = uint16(65535);
    fd.filter.groupIndex = int16(0);
    b2PolygonShape shape;
    b2Vec2 vs[8];
    vs[0].Set(1.011005401611328e+02f, 1.436634063720703e+01f);
    vs[1].Set(9.884549713134766e+01f, 2.050000000000000e+01f);
    vs[2].Set(8.613860321044922e+01f, 1.436503219604492e+01f);
    shape.Set(vs, 3);

    fd.shape = &shape;

    bodies[1]->CreateFixture(&fd);
  }
  {
    b2FixtureDef fd;
    fd.friction = 8.000000119209290e-01f;
    fd.restitution = 0.000000000000000e+00f;
    fd.density = 1.000000000000000e+00f;
    fd.isSensor = bool(0);
    fd.filter.categoryBits = uint16(1);
    fd.filter.maskBits = uint16(65535);
    fd.filter.groupIndex = int16(0);
    b2PolygonShape shape;
    b2Vec2 vs[8];
    vs[0].Set(9.884549713134766e+01f, 2.050000000000000e+01f);
    vs[1].Set(8.613860321044922e+01f, 2.049869155883789e+01f);
    vs[2].Set(8.613860321044922e+01f, 1.436503219604492e+01f);
    shape.Set(vs, 3);

    fd.shape = &shape;

    bodies[1]->CreateFixture(&fd);
  }
}
{
  b2BodyDef bd;
  bd.type = b2BodyType(2);
  bd.position.Set(-5.157864761352539e+01f, 3.655048012733459e-01f);
  bd.angle = 1.018781280517578e+01f;
  bd.linearVelocity.Set(4.370459914207458e-02f, 0.000000000000000e+00f);
  bd.angularVelocity = -1.099775731563568e-01f;
  bd.linearDamping = 0.000000000000000e+00f;
  bd.angularDamping = 0.000000000000000e+00f;
  bd.allowSleep = bool(4);
  bd.awake = bool(2);
  bd.fixedRotation = bool(0);
  bd.bullet = bool(0);
  bd.active = bool(32);
  bd.gravityScale = 1.000000000000000e+00f;
  bodies[2] = m_world->CreateBody(&bd);

  {
    b2FixtureDef fd;
    fd.friction = 8.999999761581421e-01f;
    fd.restitution = 0.000000000000000e+00f;
    fd.density = 1.500000000000000e+00f;
    fd.isSensor = bool(0);
    fd.filter.categoryBits = uint16(1);
    fd.filter.maskBits = uint16(65533);
    fd.filter.groupIndex = int16(0);
    b2CircleShape shape;
    shape.m_radius = 4.000000059604645e-01f;
    shape.m_p.Set(0.000000000000000e+00f, 0.000000000000000e+00f);

    fd.shape = &shape;

    bodies[2]->CreateFixture(&fd);
  }
}
{
  b2BodyDef bd;
  bd.type = b2BodyType(2);
  bd.position.Set(-5.256041336059570e+01f, 8.890351057052612e-01f);
  bd.angle = 6.247134208679199e+00f;
  bd.linearVelocity.Set(1.859417408704758e-01f, 4.062659144401550e-01f);
  bd.angularVelocity = -1.045708358287811e-01f;
  bd.linearDamping = 0.000000000000000e+00f;
  bd.angularDamping = 0.000000000000000e+00f;
  bd.allowSleep = bool(4);
  bd.awake = bool(2);
  bd.fixedRotation = bool(0);
  bd.bullet = bool(0);
  bd.active = bool(32);
  bd.gravityScale = 1.000000000000000e+00f;
  bodies[3] = m_world->CreateBody(&bd);

  {
    b2FixtureDef fd;
    fd.friction = 5.000000000000000e-01f;
    fd.restitution = 0.000000000000000e+00f;
    fd.density = 1.000000000000000e+00f;
    fd.isSensor = bool(0);
    fd.filter.categoryBits = uint16(1);
    fd.filter.maskBits = uint16(65533);
    fd.filter.groupIndex = int16(0);
    b2PolygonShape shape;
    b2Vec2 vs[8];
    vs[0].Set(1.500000000000000e+00f, -5.000000000000000e-01f);
    vs[1].Set(1.500000000000000e+00f, 0.000000000000000e+00f);
    vs[2].Set(0.000000000000000e+00f, 8.999999761581421e-01f);
    vs[3].Set(-1.149999976158142e+00f, 8.999999761581421e-01f);
    vs[4].Set(-1.500000000000000e+00f, 2.000000029802322e-01f);
    vs[5].Set(-1.500000000000000e+00f, -5.000000000000000e-01f);
    shape.Set(vs, 6);

    fd.shape = &shape;

    bodies[3]->CreateFixture(&fd);
  }
}
{
  b2BodyDef bd;
  bd.type = b2BodyType(2);
  bd.position.Set(-5.357979202270508e+01f, 3.696106076240540e-01f);
  bd.angle = 6.526381015777588e+00f;
  bd.linearVelocity.Set(3.949822857975960e-02f, 0.000000000000000e+00f);
  bd.angularVelocity = -9.887693822383881e-02f;
  bd.linearDamping = 0.000000000000000e+00f;
  bd.angularDamping = 0.000000000000000e+00f;
  bd.allowSleep = bool(4);
  bd.awake = bool(2);
  bd.fixedRotation = bool(0);
  bd.bullet = bool(0);
  bd.active = bool(32);
  bd.gravityScale = 1.000000000000000e+00f;
  bodies[4] = m_world->CreateBody(&bd);

  {
    b2FixtureDef fd;
    fd.friction = 8.999999761581421e-01f;
    fd.restitution = 0.000000000000000e+00f;
    fd.density = 1.000000000000000e+00f;
    fd.isSensor = bool(0);
    fd.filter.categoryBits = uint16(1);
    fd.filter.maskBits = uint16(65533);
    fd.filter.groupIndex = int16(0);
    b2CircleShape shape;
    shape.m_radius = 4.000000059604645e-01f;
    shape.m_p.Set(0.000000000000000e+00f, 0.000000000000000e+00f);

    fd.shape = &shape;

    bodies[4]->CreateFixture(&fd);
  }
}
{
  b2WheelJointDef jd;
  jd.bodyA = bodies[3];
  jd.bodyB = bodies[2];
  jd.collideConnected = bool(0);
  jd.localAnchorA.Set(1.000000000000000e+00f, -6.000000238418579e-01f);
  jd.localAnchorB.Set(0.000000000000000e+00f, 0.000000000000000e+00f);
  jd.localAxisA.Set(0.000000000000000e+00f, 1.000000000000000e+00f);
  jd.enableMotor = bool(1);
  jd.motorSpeed = 0.000000000000000e+00f;
  jd.maxMotorTorque = 2.000000000000000e+01f;
  jd.frequencyHz = 3.799999952316284e+00f;
  jd.dampingRatio = 6.999999880790710e-01f;
  joints[0] = m_world->CreateJoint(&jd);
}
{
  b2WheelJointDef jd;
  jd.bodyA = bodies[3];
  jd.bodyB = bodies[4];
  jd.collideConnected = bool(0);
  jd.localAnchorA.Set(-9.999982118606567e-01f, -6.499999761581421e-01f);
  jd.localAnchorB.Set(0.000000000000000e+00f, 0.000000000000000e+00f);
  jd.localAxisA.Set(0.000000000000000e+00f, 1.000000000000000e+00f);
  jd.enableMotor = bool(1);
  jd.motorSpeed = 0.000000000000000e+00f;
  jd.maxMotorTorque = 2.000000000000000e+01f;
  jd.frequencyHz = 3.799999952316284e+00f;
  jd.dampingRatio = 6.999999880790710e-01f;
  joints[1] = m_world->CreateJoint(&jd);
}
b2Free(joints);
b2Free(bodies);
joints = NULL;
bodies = NULL;

