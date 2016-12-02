/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 *
 * Copyright (c) 2015-2016 Inviwo Foundation
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

#ifndef IVW_INVIWOMODULEFACTORYOBJECT_H
#define IVW_INVIWOMODULEFACTORYOBJECT_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/util/stdextensions.h>

namespace inviwo {

class InviwoModule;
class InviwoApplication;

class IVW_CORE_API InviwoModuleFactoryObject {
public:
    InviwoModuleFactoryObject(const std::string& name, const std::string& version,
                              const std::string& description, const std::string& inviwoCoreVersion,
                              std::vector<std::string> dependencies,
                              std::vector<std::string> dependenciesVersion);
    virtual ~InviwoModuleFactoryObject() = default;

    virtual std::unique_ptr<InviwoModule> create(InviwoApplication* app) = 0;

    const std::string name;                  // Module name
    const std::string version;               // Module version (Major.Minor.Patch)
    const std::string description;           // Module description
    const std::string inviwoCoreVersion;     // Supported inviwo core version (Major.Minor.Patch)
    const std::vector<std::string> dependencies;        // Module dependencies
    const std::vector<std::string> dependenciesVersion; // Major.Minor.Patch version of each dependency
};

template <typename T>
class InviwoModuleFactoryObjectTemplate : public InviwoModuleFactoryObject {
public:
    InviwoModuleFactoryObjectTemplate(const std::string& name, const std::string& version,
                                      const std::string& description,
                                      const std::string& inviwoCoreVersion,
                                      std::vector<std::string> dependencies,
                                      std::vector<std::string> dependenciesVersion);

    virtual std::unique_ptr<InviwoModule> create(InviwoApplication* app) override {
        return util::make_unique<T>(app);
    }
};

// Function pointer for exported module factory creation function in dynamic library.
// Will be generated by globalmacros.cmake in function ivw_private_generate_module_registration_file
// Returns a pointer to a InviwoModuleFactoryObject without any input arguments
#ifdef WIN32
// __stdcall is the convention used by the WinAPI
typedef InviwoModuleFactoryObject*(__stdcall* f_getModule)();
#else
typedef InviwoModuleFactoryObject*(* f_getModule)();
#endif

template <typename T>
inviwo::InviwoModuleFactoryObjectTemplate<T>::InviwoModuleFactoryObjectTemplate(
    const std::string& name, const std::string& version, const std::string& description,
    const std::string& inviwoCoreVersion, std::vector<std::string> dependencies,
    std::vector<std::string> dependenciesVersion)
    : InviwoModuleFactoryObject(name, version, description, inviwoCoreVersion, dependencies,
                                dependenciesVersion) {}

/**
 * \brief Topological sort to make sure that we load modules in correct order
 *
 * https://en.wikipedia.org/wiki/Topological_sorting#Depth-first_search
 *
 * @param std::vector<std::unique_ptr<InviwoModuleFactoryObject>>::iterator start Start of range to
 * sort
 * @param std::vector<std::unique_ptr<InviwoModuleFactoryObject>>::iterator end End of range to sort
 */
IVW_CORE_API void topologicalModuleFactoryObjectSort(
    std::vector<std::unique_ptr<InviwoModuleFactoryObject>>::iterator start,
    std::vector<std::unique_ptr<InviwoModuleFactoryObject>>::iterator end);

}  // namespace

#endif  // IVW_INVIWOMODULEFACTORYOBJECT_H
