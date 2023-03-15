#include "vulkan/vulkan.h"

namespace CrystalEye 
{
   class CEDevice
   {
   public:
      CEDevice();
      void Initialize(VkInstance Instance);
      VkDevice Device;
   };
}
