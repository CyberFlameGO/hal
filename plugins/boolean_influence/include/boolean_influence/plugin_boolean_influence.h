//  MIT License
//
//  Copyright (c) 2019 Ruhr University Bochum, Chair for Embedded Security. All Rights reserved.
//  Copyright (c) 2021 Max Planck Institute for Security and Privacy. All Rights reserved.
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in all
//  copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//  SOFTWARE.

#pragma once

#include "hal_core/netlist/gate.h"
#include "hal_core/netlist/net.h"
#include "hal_core/plugin_system/plugin_interface_base.h"

namespace hal
{
    class PLUGIN_API BooleanInfluencePlugin : public BasePluginInterface
    {
    public:
        std::string get_name() const override;
        std::string get_version() const override;

        void initialize() override;

        /**
         * Generates the function of the dataport net of the given flip-flop.
         * Afterwards the generated function gets translated from a z3::expr to efficent c code, compiled, executed and evalated.G
         *
         * @param[in] gate - Pointer to the flip-flop which data input net is used to build the boolean function.
         * @returns A mapping of the gates that appear in the function of the data net to their boolean influence in said function.
         */
        std::map<Net*, double> get_boolean_influences_of_gate(const Gate* gate);

    private:
        std::vector<Gate*> extract_function_gates(const Gate* start, const std::string& pin);
        void add_inputs(Gate* gate, std::unordered_set<Gate*>& gates);
    };
}    // namespace hal
