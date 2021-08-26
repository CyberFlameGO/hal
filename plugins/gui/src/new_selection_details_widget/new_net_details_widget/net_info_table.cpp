#include "gui/new_selection_details_widget/new_net_details_widget/net_info_table.h"

#include "hal_core/netlist/net.h"

#include "gui/gui_globals.h"
#include "gui/netlist_relay/netlist_relay.h"
#include "gui/new_selection_details_widget/py_code_provider.h"

#include <QDebug>
#include <QMenu>
#include <QApplication>
#include <QClipboard>

namespace hal
{
    NetInfoTable::NetInfoTable(QWidget* parent) : GeneralTableWidget(parent)
    {
        mNameEntryContextMenu = new QMenu();
        mNameEntryContextMenu->addAction("changeName", std::bind(&NetInfoTable::changeName, this));
        mNameEntryContextMenu->addAction("copyName", std::bind(&NetInfoTable::copyName, this));
        mNameEntryContextMenu->addAction("pyCopyName", std::bind(&NetInfoTable::pyCopyName, this));

        mIdEntryContextMenu = new QMenu();
        mIdEntryContextMenu->addAction("copyId", std::bind(&NetInfoTable::copyId, this));

        mTypeEntryContextMenu = new QMenu();
        mTypeEntryContextMenu->addAction("copyType", std::bind(&NetInfoTable::copyType, this));
        mTypeEntryContextMenu->addAction("pyCopyType", std::bind(&NetInfoTable::pyCopyType, this));

        mNumSrcsEntryContextMenu = new QMenu();
        mNumSrcsEntryContextMenu->addAction("copyNumSrcs", std::bind(&NetInfoTable::copyNumberOfSrcs, this));

        mNumDstsEntryContextMenu = new QMenu();
        mNumDstsEntryContextMenu->addAction("copyNumDsts", std::bind(&NetInfoTable::copyNumberOfDsts, this));

        connect(gNetlistRelay, &NetlistRelay::netRemoved, this, &NetInfoTable::handleNetRemoved);
        connect(gNetlistRelay, &NetlistRelay::netNameChanged, this, &NetInfoTable::handleNetNameChanged);
        connect(gNetlistRelay, &NetlistRelay::netSourceAdded, this, &NetInfoTable::handleSrcDstChanged);
        connect(gNetlistRelay, &NetlistRelay::netSourceRemoved, this, &NetInfoTable::handleSrcDstChanged);
        connect(gNetlistRelay, &NetlistRelay::netDestinationAdded, this, &NetInfoTable::handleSrcDstChanged);
        connect(gNetlistRelay, &NetlistRelay::netDestinationRemoved, this, &NetInfoTable::handleSrcDstChanged);
        connect(gNetlistRelay, &NetlistRelay::netlistMarkedGlobalInput, this, &NetInfoTable::handleNetTypeChanged);
        connect(gNetlistRelay, &NetlistRelay::netlistUnmarkedGlobalInput, this, &NetInfoTable::handleNetTypeChanged);
        connect(gNetlistRelay, &NetlistRelay::netlistMarkedGlobalOutput, this, &NetInfoTable::handleNetTypeChanged);
        connect(gNetlistRelay, &NetlistRelay::netlistUnmarkedGlobalOutput, this, &NetInfoTable::handleNetTypeChanged);
        connect(gNetlistRelay, &NetlistRelay::netlistMarkedGlobalInout, this, &NetInfoTable::handleNetTypeChanged);
        connect(gNetlistRelay, &NetlistRelay::netlistUnmarkedGlobalInout, this, &NetInfoTable::handleNetTypeChanged);
    }

    void NetInfoTable::setNet(hal::Net* net)
    {
        if(gNetlist->is_net_in_netlist(net))
        {
            mNet = net;

            setRow("Name", name(), mNameEntryContextMenu);
            setRow("Id", id(), mIdEntryContextMenu);
            setRow("Type", type(), mTypeEntryContextMenu);
            setRow("No. of Sources", numberOfSrcs(), mNumSrcsEntryContextMenu);
            setRow("No. of Destinations", numberOfDsts(), mNumDstsEntryContextMenu);

            adjustSize();
        }
    }

    QString NetInfoTable::name() const
    {
        return QString::fromStdString(mNet->get_name());
    }

    QString NetInfoTable::id() const
    {
        return QString::number(mNet->get_id());
    }

    QString NetInfoTable::type() const
    {
        QString type = "";

        if(mNet->is_global_input_net())
            type = "Global input";
        else if(mNet->is_global_output_net())
            type = "Global output";
        else if(mNet->is_unrouted())
            type = "Unrouted";
        else
            type = "Internal";
        
        return type;
    }

    QString NetInfoTable::numberOfSrcs() const
    {
        return QString::number(mNet->get_num_of_sources());
    }

    QString NetInfoTable::numberOfDsts() const
    {
        return QString::number(mNet->get_num_of_destinations());
    }

    void NetInfoTable::changeName()
    {
        qDebug() << "changeName";
    }

    void NetInfoTable::copyName() const
    {
        copyToClipboard(name());
    }

    void NetInfoTable::pyCopyName() const
    {
        copyToClipboard(PyCodeProvider::pyCodeNetName(mNet->get_id()));
    }

    void NetInfoTable::copyId() const
    {
        copyToClipboard(id());
    }

    void NetInfoTable::copyType() const
    {
        copyToClipboard(type());
    }
    
    void NetInfoTable::pyCopyType() const
    {
        copyToClipboard(PyCodeProvider::pyCodeNetType(mNet->get_id()));
    }
    
    void NetInfoTable::copyNumberOfSrcs() const
    {
        copyToClipboard(numberOfSrcs());
    }

    void NetInfoTable::copyNumberOfDsts() const
    {
        copyToClipboard(numberOfDsts());
    }

    void NetInfoTable::handleNetRemoved(Net* net)
    {
        if(mNet == net)
        {
            mNet = nullptr;

            const QString notification("Displayed net has been deleted.");

            setRow("Name", notification, nullptr);
            setRow("Id", notification, nullptr);
            setRow("Type", notification, nullptr);
            setRow("No. of Sources", notification, nullptr);
            setRow("No. of Destinations", notification, nullptr);

            adjustSize();
        }
    }

    void NetInfoTable::handleNetNameChanged(Net* net)
    {
        if(mNet == net)
            refresh();
    }

    void NetInfoTable::handleNetTypeChanged(Netlist* netlist, const u32 netId)
    {
        Q_UNUSED(netlist)

        if(mNet->get_id() == netId)
            refresh();
    }

    void NetInfoTable::handleSrcDstChanged(Net* net, u32 srcDstGateId)
    {
        Q_UNUSED(srcDstGateId)

        if(mNet == net)
            refresh();
    }

    void NetInfoTable::refresh()
    {
        setNet(mNet);
    }
}
