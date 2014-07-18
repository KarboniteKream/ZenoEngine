#include "particle.h"

void initParticleSystem(ParticleSystem *particleSystem, GLfloat x, GLfloat y, GLfloat wh, int particleNumber)
{
	particleSystem->X = x;
	particleSystem->Y = y;
	particleSystem->WH = wh;

	particleSystem->NumberOfParticles = particleNumber;

	particleSystem->Particles = (Particle *)malloc(particleNumber * sizeof(Particle));

	particleSystem->ParticleTextures = (Texture *)malloc(1 * sizeof(Texture));

	for(int i = 0; i < 1; i++)
	{
		initTexture(&particleSystem->ParticleTextures[0]);
		loadTexture(&particleSystem->ParticleTextures[0], "images/leaf.png");
	}

	for(int i = 0; i < particleNumber; i++)
	{
		// NOTE: What about scale?
		GLfloat particleY = rand() % (int)wh + y;
		// NOTE: The speed of groups of particles seems exactly the same.
		GLfloat speedX = (float)rand() / (float)RAND_MAX * 500.0f + 10.0f;
		initParticle(&particleSystem->Particles[i], x, particleY, -speedX);
	}
}

void initParticle(Particle *particle, GLfloat x, GLfloat y, GLfloat speedX)
{
	particle->TextureID = 0;

	particle->X = x;
	particle->Y = y;

	particle->SpeedX = speedX;

	// NOTE: A state variable?
	particle->TTL = -1;
}

void setParticleTTL(ParticleSystem *particleSystem, unsigned int ttl)
{
	for(unsigned int i = 0; i < particleSystem->NumberOfParticles; i++)
	{
		particleSystem->Particles[i].TTL = ttl;
	}
}

void debugParticleSystem(ParticleSystem *particleSystem)
{
	drawRectangle(particleSystem->X, particleSystem->Y, 5.0f, particleSystem->WH, 1.0f, 1.0f, 1.0f, 1.0f);
}

void updateParticleSystem(ParticleSystem *particleSystem)
{
	for(unsigned int i = 0; i < particleSystem->NumberOfParticles; i++)
	{
		if(particleSystem->Particles[i].TTL != -1)
		{
			if(particleSystem->Particles[i].X < 0.0f /* - WIDTH */)
			{
				particleSystem->Particles[i].X = particleSystem->X;
			}
			else
			{
				particleSystem->Particles[i].X += particleSystem->Particles[i].SpeedX * DELTA_TICKS;
				drawParticle(particleSystem, &particleSystem->Particles[i]);
			}
		}
	}
}

void drawParticle(ParticleSystem *particleSystem, Particle *particle)
{
	drawTexture(&particleSystem->ParticleTextures[particle->TextureID], particle->X, particle->Y, NULL, 0.0f, 4.0f, false);
}
