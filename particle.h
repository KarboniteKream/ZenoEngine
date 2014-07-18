#ifndef PARTICLE_H
#define PARTICLE_H

#include "globals.h"
#include "texture.h"
#include "util.h"

typedef struct
{
	unsigned int TextureID;

	GLfloat X;
	GLfloat Y;

	GLfloat SpeedX;

	int TTL;
} Particle;

typedef struct
{
	// NOTE: RectangleF instead?
	GLfloat X;
	GLfloat Y;
	// NOTE: This is currently only height.
	GLfloat WH;

	// NOTE: How many particles do I really need?
	unsigned int NumberOfParticles;

	Texture *ParticleTextures;
	Particle *Particles;

} ParticleSystem;

void initParticleSystem(ParticleSystem *particleSystem, GLfloat x, GLfloat y, GLfloat length, int particleNumber);
void initParticle(Particle *particle, GLfloat x, GLfloat y, GLfloat speedX);
void setParticleTTL(ParticleSystem *particleSystem, unsigned int ttl);
void debugParticleSystem(ParticleSystem *particleSystem);
void updateParticleSystem(ParticleSystem *particleSystem);
void drawParticle(ParticleSystem *particleSystem, Particle *particle);

#endif
