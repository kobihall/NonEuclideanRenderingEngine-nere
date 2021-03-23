//
//  vulkan_initializers.hpp
//  NonEuclideanRenderingEngine-nere main file
//
//  Created by Kobi Hall on 3/23/21.
//  Copyright © 2021 Kobi Hall. All rights reserved.
//

#pragma once

#include "nere_device.hpp"

//std
#include <vector>

namespace nere {

namespace initializers {

inline static void check_vk_result(VkResult err)
{
    if (err == 0)
        return;
    fprintf(stderr, "[vulkan] Error: VkResult = %d\n", err);
    if (err < 0)
        abort();
}

}
}