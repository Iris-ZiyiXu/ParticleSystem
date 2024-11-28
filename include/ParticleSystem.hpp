#ifndef PARTICLESYSTEM_HPP
#define PARTICLESYSTEM_HPP

#if defined(LINUX) || defined(MINGW)
    #include <SDL2/SDL.h>
#else // This works for Mac
    #include <SDL.h>
#endif

#include "Shader.hpp"
#include <glm/gtc/type_ptr.hpp>
#include "VertexBufferLayout.hpp"

#include <glm/glm.hpp>
#include <vector>

struct ParticleProps
{
  glm::vec2 Position;
  glm::vec2 Velocity, VelocityVariation;
  glm::vec4 ColorBegin, ColorEnd;
  float SizeBegin, SizeEnd, SizeVariation;
  float LifeTime = 1.0f;
};

class ParticleSystem
{
public:
  ParticleSystem();

  void Update(float timestep);
  void Init();
  void Render(unsigned int screenWidth, unsigned int screenHeight);
  void Emit(const ParticleProps &particleProps);

private:
  struct Particle
  {
    glm::vec2 Position;
    glm::vec2 Velocity;
    glm::vec4 ColorBegin, ColorEnd;
    float SizeBegin, SizeEnd;
    float LifeTime = 1.0f;
    float LifeRemaining = 0.0f;

    bool active = false;
  };
  std::vector<Particle> m_ParticlePool;
  uint32_t m_PoolIndex = 999;

  Shader m_shader;
  VertexBufferLayout m_vertexBufferLayout;

  std::vector<float> m_vertices;
  std::vector<unsigned int> m_indices;
  glm::mat4 m_projectionMatrix;

};

#endif