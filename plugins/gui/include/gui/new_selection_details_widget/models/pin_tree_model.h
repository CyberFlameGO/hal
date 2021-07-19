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

#include "gui/new_selection_details_widget/models/base_tree_model.h"
#include <QMap>

namespace hal
{

    class Gate;

/**
 * @ingroup gui
 * @brief A model to display the pins of a gate.
 */
class PinTreeModel : public BaseTreeModel
{
    Q_OBJECT

public:

    /**
     * The constructor.
     *
     * @param parent - The model's parent.
     */
    PinTreeModel(QObject* parent = nullptr);

    /**
      * The destructor.
      */
    ~PinTreeModel();

    /**
     * Overwritten BaseTreeModel function.
     */
    void clear() override;

    /**
     * Sets the gate of the pins this model will represent.
     *
     * @param g - The gate of the pins.
     */
    void setGate(Gate* g);

    /**
     * Get the ID of the gate that is currently displayed. If no gate is
     * currently displayed, -1 is returned.
     *
     * @return The gate's id.
     */
    int getCurrentGateID();

    //column identifier
    static const int sNameColumn = 0;
    static const int sDirectionColumn = 1;
    static const int sTypeColumn = 2;
    static const int sConnectedNetColumn = 3;

    //additional data keys
    const QString keyType = "type";
    const QString keyRepresentedNetID = "netID"; //might not be needed

    enum itemType {grouping = 0, pin = 1};
    Q_ENUM(itemType)

private:
    int mGateId;
    QMap<std::string, TreeItem*> mPinGroupingToTreeItem;

    //helper method to prevent code redundance
    void appendInputOutputPins(Gate* g, std::vector<std::string> inputOrOutputPins, bool areInputPins);

};

}
