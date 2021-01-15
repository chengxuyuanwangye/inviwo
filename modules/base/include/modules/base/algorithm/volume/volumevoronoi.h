/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 *
 * Copyright (c) 2020-2021 Inviwo Foundation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *********************************************************************************/
#pragma once

#include <modules/base/basemoduledefine.h>
#include <inviwo/core/datastructures/volume/volume.h>
#include <inviwo/core/datastructures/volume/volumeramprecision.h>
#include <inviwo/core/util/indexmapper.h>
#include <inviwo/core/util/glm.h>
#include <inviwo/core/util/zip.h>
#include <inviwo/core/util/volumeramutils.h>
#include <vector>

namespace inviwo {
namespace util {

/**
 * Implementation of Voronoi segmentation.
 *
 * The function returns a volume with each voxel containing the index for the closest seed point
 * (according to the power distance with or without weights).
 *
 *     * volumeDimensions is the dimensions for the volume.
 *     * indexToModelMatrix is the matrix to transform the voxel positions from index to model
 *       space.
 *     * seedPointsWithIndices is a vector containing the seed points for the algorithm together
 *       with their index number on the form {index, position}. The positions are expected be in
 *       model space.
 *     * weigths is an optional vector containing the weights for each seed point.
 *     * weightedVoronoi is a boolean deciding if the weighted version of voronoi should be used.
 *       If true, the weights must be provided.
 */

IVW_MODULE_BASE_API std::shared_ptr<Volume> voronoiSegmentation(
    const size3_t volumeDimensions, const mat4& indexToModelMatrix,
    const std::vector<std::pair<uint32_t, vec3>>& seedPointsWithIndices,
    const std::optional<std::vector<float>>& weights, bool weightedVoronoi);

}  // namespace util
}  // namespace inviwo