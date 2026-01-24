#pragma once

#include <cstdint>
#include <limits>

// Indices (locations) of Queue Families (if they exist at all)
struct QueueFamilyIndices {
    // Location of Graphics Queue Family
    uint32_t graphicsFamily = UINT32_MAX;

    // Check if queue families are valid
    bool isValid() const
    {
        return graphicsFamily != UINT32_MAX;
    }
};
