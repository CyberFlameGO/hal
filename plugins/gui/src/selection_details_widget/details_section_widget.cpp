#include "gui/gui_globals.h"
#include "gui/selection_details_widget/details_section_widget.h"
#include "gui/selection_details_widget/details_table_utilities.h"
#include <QVBoxLayout>
#include <QHeaderView>

namespace hal {

    DetailsSectionWidget::DetailsSectionWidget(QWidget* content, const QString &txt, QWidget *parent)
        : QWidget(parent), mBody(content), mTable(nullptr)
    {
        constructor(txt);
        mLayout->addWidget(mBody);
    }

    DetailsSectionWidget::DetailsSectionWidget(const QString &txt, QTableWidget* tab, QWidget *parent)
        : QWidget(parent), mBody(tab), mTable(tab)
    {
        Q_ASSERT (tab != nullptr);

        constructor(txt);

        QHBoxLayout *hlayout = new QHBoxLayout;
        hlayout->setContentsMargins(3,3,0,0);
        hlayout->setSpacing(10);
        hlayout->addWidget(tab);
        hlayout->addSpacerItem(new QSpacerItem(0,0, QSizePolicy::Expanding, QSizePolicy::Fixed));
        mLayout->addLayout(hlayout);
        mLayout->addSpacerItem(new QSpacerItem(0,7, QSizePolicy::Expanding, QSizePolicy::Fixed));

        DetailsTableUtilities::setDefaultTableStyle(mTable);
    }

    void DetailsSectionWidget::constructor(const QString &txt)
    {
        Q_ASSERT (mBody != nullptr);

        mRows        = 0;
        mBodyVisible = true;
        mHeaderText  = txt;
        mHideEmpty   = false;
        mLayout      = new QVBoxLayout(this);

        // remove placeholder text from initial view
        mHeader      = new QPushButton(txt.endsWith(" (%1)") ? txt.mid(0,txt.size()-5) : txt,this);
        mLayout->setMargin(0);
        mLayout->addWidget(mHeader);

        connect(mHeader,&QPushButton::clicked,this,&DetailsSectionWidget::toggleBodyVisible);
    }

    QTableWidget* DetailsSectionWidget::table() const
    {
        return mTable;
    }

    void DetailsSectionWidget::hideEmpty()
    {
        if (mHideEmpty && mRows == 0)
            hide();
        else
        {
            show();
            bodyVisible();
        }
    }

    void DetailsSectionWidget::setRowCount(int rc)
    {
        mBodyVisible = true; // show body after each update
        mRows = rc;
        if (mHeaderText.contains("%1"))
            mHeader->setText(QString(mHeaderText).arg(mRows));
        hideEmpty();
    }

    void DetailsSectionWidget::toggleBodyVisible()
    {
        mBodyVisible = ! mBodyVisible;
        bodyVisible();
    }

    void DetailsSectionWidget::bodyVisible()
    {
        if (mBodyVisible)
            mBody->show();
        else
            mBody->hide();
    }

    void DetailsSectionWidget::hideWhenEmpty(bool hide)
    {
        mHideEmpty = hide;
        hideEmpty();
    }
}
