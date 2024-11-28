#include "ParticleSystem.hpp"

#include "Random.h"
#include "Transform.hpp"

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