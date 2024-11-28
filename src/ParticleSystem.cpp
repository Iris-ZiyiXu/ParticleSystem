#include "ParticleSystem.hpp"

#include "Random.h"
#include "Camera.hpp"

#include <iostream>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/compatibility.hpp>
#define GLM_ENABLE_EXPERIMENTAL

ParticleSystem::ParticleSystem()
{
  m_ParticlePool.resize(1000);
}

void ParticleSystem::Update(float timestep)
{
  m_vertexBufferLayout.Bind();
  m_shader.Bind();
  for (auto& particle : m_PoolIndex)
  {
    // If particle is unused, skip
    if (!particle.active)
    {
      continue;
    }
    // If particle is dead, skip
    if (particle.lifeRemaining <= 0.0f)
    {
      particle.active = false;
      continue;
    }
    // Update each active particle
    particle.lifeRemaining -= timestep;
    particle.Position += particle.Velocity * timestep;
    particle.Velocity.y += (0.01f * timestep);
  }
}

void ParticleSystem::Init()
{
  // vertices for quad
  m_vertices = 
  {
    -0.1f, -0.1f, 0.0f,
    0.1f,  -0.1f, 0.0f,
     0.1f,  0.1f, 0.0f,
     -0.1f, 0.1f, 0.0f
  };
  // indices for quad
  m_indices = 
  {
    0, 1, 2,
    2, 3, 0
  };

  // vertexBuffer setup
  m_vertexBufferLayout.CreatePositionBufferLayout(
    m_vertices.size(), m_indices.size(), m_vertices.data(), m_indices.data()
  );

  // shader setup
  std::string vertexShader = m_shader.LoadShader("./shaders/vert.glsl");
  std::string fragmentShader = m_shader.LoadShader("./shaders/frag.glsl");

  m_shader.CreateShader(vertexShader, fragmentShader);
}

void ParticleSystem::Render(unsigned int screenWidth, unsigned int screenHeight)
{
  for (auto& particle : m_PoolIndex)
  {
    // If particle is unused, skip
    if (!particle.active)
    {
      continue;
    }
    
    float particleLife = particle.LifeRemaining / particle.LifeTime;
    glm::vec4 particleColor = glm::lerp(
      particle.ColorEnd, particle.ColorBegin, particleLife
    );
    float particleSize = glm::lerp(
      particle.SizeEnd, particle.SizeBegin, particleLife
    );

    m_projectionMatrix = glm::perspective(
                    45.0f, 
                    (float)screenWidth / (float)screenHeight, 
                    0.1f, 512.0f);

    // convert screen coordinates to world coordinates
    float x = 2.0 * particle.Position.x / screenWidth - 1.0;
    float y = 2.0 * particle.Position.y / screenHeight - 1.0;
    // Homogeneous Coordinates
    glm::vec4 screenPos = glm::vec4(x, -y, -1.0f, 1.0f);
    // Projection space
    glm::mat4 projectView = m_projectionMatrix * Camera::GetWorldToViewmatrix();
    glm::mat4 viewProjectionInverse = glm::inverse(projectView);
    glm::vec4 worldPos = viewProjectionInverse * screenPos;

    // Model transformation
    m_transform.LoadIdentity();
    m_transform.Translate(worldPos[0], worldPos[1], worldPos[2]);
    m_transform.Rotate(particle.rotation, 0.0f, 0.0f, 1.0f);
    m_transform.Scale(particleSize, particleSize, 1.0f);

    // Set shader uniforms
    m_shader.SetUniformMatrix4fv("modelTransformMatrix",
                      m_transform.GetTransformMatrix());
    m_shader.SetUniformMatrix4fv("viewMatrix",
                      &Camera::GetWorldToViewmatrix()[0][0]);
    m_shader.SetUniformMatrix4fv("projectionMatrix",
                      &m_projectionMatrix[0][0]);
    m_shader.SetUniform4f("color",
                    particleColor[0],
                    particleColor[1],
                    particleColor[2],
                    particleColor[3]);

    // Draw the particles based on the shape
    if (particle.shape == ParticleShape::TRIANGLE)
    {
      glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
    }
    else
    {
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }
  }
}

void ParticleSystem::Emit(const ParticleProps &particleProps)
{
  Particle& particle = m_ParticlePool[m_PoolIndex];
  particle.active = true;
  particle.Position = particleProps.Position;
  particle.rotation = Random::Float() * 2.0f * glm::pi<float>();

  particle.Velocity = particleProps.Velocity;
  particle.Velocity.x += particleProps.VelocityVariation.x * (Random::Float() - 0.5f);
  particle.Velocity.y += particleProps.VelocityVariation.y * (Random::Float() - 0.5f);

  particle.ColorBegin = particleProps.ColorBegin;
  particle.ColorEnd = particleProps.ColorEnd;

  particle.SizeBegin = particleProps.SizeBegin + particleProps.SizeVariation * (Random::Float() - 0.5f);
  particle.SizeEnd = particleProps.SizeEnd;
  particle.shape = particleProps.shape;

  particle.LifeTime = particleProps.LifeTime;
  particle.LifeRemaining = particleProps.LifeTime;

  particle.active = true;
  m_PoolIndex = --m_PoolIndex % m_ParticlePool.size();
}