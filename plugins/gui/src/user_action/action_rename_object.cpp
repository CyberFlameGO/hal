#include "gui/gui_globals.h"
#include "gui/user_action/action_rename_object.h"
#include "gui/graph_widget/contexts/graph_context.h"
#include "gui/grouping/grouping_manager_widget.h"
#include "gui/grouping/grouping_table_model.h"

namespace hal
{
    ActionRenameObjectFactory::ActionRenameObjectFactory()
       : UserActionFactory("RenameObject") {;}

    ActionRenameObjectFactory* ActionRenameObjectFactory::sFactory = new ActionRenameObjectFactory;

    UserAction* ActionRenameObjectFactory::newAction() const
    {
        return new ActionRenameObject;
    }

    QString ActionRenameObject::tagname() const
    {
        return ActionRenameObjectFactory::sFactory->tagname();
    }

    void ActionRenameObject::addToHash(QCryptographicHash& cryptoHash) const
    {
        cryptoHash.addData(mNewName.toUtf8());
    }

    void ActionRenameObject::writeToXml(QXmlStreamWriter& xmlOut) const
    {
        xmlOut.writeTextElement("name", mNewName);
        if (mObject.type()==UserActionObjectType::Port && mPortType != NoPort)
        {
            xmlOut.writeTextElement("netid", QString::number(mNetId));
            xmlOut.writeTextElement("porttype", mPortType==Input ? "input" : "output");
        }
    }

    void ActionRenameObject::readFromXml(QXmlStreamReader& xmlIn)
    {
        while (xmlIn.readNextStartElement())
        {
            if (xmlIn.name() == "name")
                mNewName = xmlIn.readElementText();
            if (xmlIn.name() == "netid")
                mNetId = xmlIn.readElementText().toInt();
            if (xmlIn.name() == "porttype")
                mPortType = (xmlIn.readElementText() == "input") ? Input : Output;
        }
    }

    bool ActionRenameObject::exec()
    {
        QString       oldName;
        Module*       mod;
        Gate*         gat;
        Net*          net;
        GraphContext* ctx;
        switch (mObject.type()) {
        case UserActionObjectType::Module:
            mod = gNetlist->get_module_by_id(mObject.id());
            if (mod)
            {
                oldName = QString::fromStdString(mod->get_name());
                mod->set_name(mNewName.toStdString());
            }
            else
                return false;
            break;
        case UserActionObjectType::Gate:
            gat = gNetlist->get_gate_by_id(mObject.id());
            if (gat)
            {
                oldName = QString::fromStdString(gat->get_name());
                gat->set_name(mNewName.toStdString());
            }
            else
                return false;
            break;
        case UserActionObjectType::Net:
            net = gNetlist->get_net_by_id(mObject.id());
            if (net)
            {
                oldName = QString::fromStdString(net->get_name());
                net->set_name(mNewName.toStdString());
            }
            else
                return false;
            break;
        case UserActionObjectType::Grouping:
            oldName = gContentManager->getGroupingManagerWidget()->getModel()->
                    renameGrouping(mObject.id(),mNewName);
            break;
        case UserActionObjectType::Context:
            ctx = gGraphContextManager->getContextById(mObject.id());
            if (ctx)
            {
                oldName = ctx->name();
                gGraphContextManager->renameGraphContextAction(ctx,mNewName);
            }
            else
                return false;
            break;
        case UserActionObjectType::Port:
            mod = gNetlist->get_module_by_id(mObject.id());
            net = gNetlist->get_net_by_id(mNetId);
            if (mod && net)
            {
                switch (mPortType) {
                case NoPort:
                    return false;
                case Input:
                    oldName = QString::fromStdString(mod->get_input_port_name(net));
                    mod->set_input_port_name(net,mNewName.toStdString());
                    break;
                case Output:
                    oldName = QString::fromStdString(mod->get_output_port_name(net));
                    mod->set_output_port_name(net,mNewName.toStdString());
                    break;
                }
            }
            else
                return false;
            break;
        default:
            return false;
        }
        ActionRenameObject* undo = new ActionRenameObject(oldName);
        undo->setObject(mObject);
        if (mPortType!=NoPort)
        {
            undo->mNetId    = mNetId;
            undo->mPortType = mPortType;
        }
        mUndoAction = undo;
        return UserAction::exec();
    }
}
