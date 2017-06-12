
inline free_camera::free_camera()
{
}

inline free_camera::~free_camera()
{
}

inline void moveForward(float dt);
inline void moveBackwards(float dt);
inline void moveUp(float dt);
inline void moveDown(float dt);
inline void moveSideLeft(float dt);
inline void moveSideRight(float dt);

inline void addYaw(float dt, float value);
inline void subtractYaw(float dt, float value);
inline void addPitch(float dt, float value);
inline void subtractPitch(float dt, float value);
inline void addRoll(float dt, float value);
inline void subtractRoll(float dt, float value);

inline float getPositionX();
inline float getPositionY();
inline float getPositionZ();

inline float getForwardX();
inline float getForwardY();
inline float getForwardZ();

inline float getLookAtX();
inline float getLookAtY();
inline float getLookAtZ();

inline float getUpX();
inline float getUpY();
inline float getUpZ();

inline float getSideX();
inline float getSideY();
inline float getSideZ();

inline float getYaw();
inline float getPitch();
inline float getRoll();

inline void setLookAtX(float x);
inline void setLookAtY(float y);
inline void setLookAtZ(float z);

inline void setYaw(float arg);
inline void setPitch(float arg);
inline void setRoll(float arg);

inline void updateYaw(int width, int mouseX, int speed);
inline void updatePitch(int height, int mouseY, int speed);
