#pragma once

#include <vector>
#include <iostream>
#include "vulkan/vulkan.h"

#define VK_CHECK(x) if((x) != VK_SUCCESS) \
	{									\
		std::cout << "Vulkan command failed with " << x << std::endl; \
		exit(0);						\
	}

const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};

#ifdef NDEBUG
    const bool bEnableValidationLayers = false;
#else
    const bool bEnableValidationLayers = true;
#endif
