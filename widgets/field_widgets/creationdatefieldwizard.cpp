/*
 *  Copyright (c) 2012 Giorgio Wicklein <giorgio.wicklein@giowisys.com>
 */

//-----------------------------------------------------------------------------
// Hearders
//-----------------------------------------------------------------------------

#include "creationdatefieldwizard.h"
#include "ui_creationdatefieldwizard.h"

#include "../../components/metadataengine.h"
#include "../../utils/metadatapropertiesparser.h"

#include <QtCore/QDateTime>
#include <QtCore/QLocale>


//-----------------------------------------------------------------------------
// Public
//-----------------------------------------------------------------------------

CreationDateFieldWizard::CreationDateFieldWizard(const QString &fieldName,
                                                 QWidget *parent,
                                                 AbstractFieldWizard::EditMode editMode) :
    AbstractFieldWizard(fieldName, parent, editMode),
    ui(new Ui::CreationDateFieldWizard)
{
    ui->setupUi(this);

    //setup format combo
    QDateTime nowDateTime = QDateTime::currentDateTime();
    QLocale locale;
    ui->dateFormatBox->addItem(nowDateTime.toString(
                                   locale.dateTimeFormat(QLocale::ShortFormat)), 1);
    ui->dateFormatBox->addItem(nowDateTime.toString(
                                   locale.dateFormat(QLocale::ShortFormat)), 2);
    ui->dateFormatBox->addItem(nowDateTime.toString("ddd MMM d hh:mm yyyy"), 3);
    ui->dateFormatBox->addItem(nowDateTime.toString("ddd MMM d yyyy"), 4);
    ui->dateFormatBox->addItem(nowDateTime.toString("yyyy-MM-dd hh:mm"), 5);
    ui->dateFormatBox->addItem(nowDateTime.toString("yyyy-MM-dd"), 6);

    connect(ui->backButton, SIGNAL(clicked()),
            this, SIGNAL(backSignal()));
    connect(ui->finishButton, SIGNAL(clicked()),
            this, SIGNAL(finishSignal()));

    ui->finishButton->setFocus();
}

CreationDateFieldWizard::~CreationDateFieldWizard()
{
    delete ui;
}

void CreationDateFieldWizard::getFieldProperties(QString &displayProperties,
                                         QString &editProperties,
                                         QString &triggerProperties)
{
    //create display properties metadata string
    int index = ui->dateFormatBox->currentIndex();
    int formatCode = ui->dateFormatBox->itemData(index).toInt();
    displayProperties.append(QString("dateFormat:%1;").arg(formatCode));

    //create edit properties metadata string
    //nothing for now
    Q_UNUSED(editProperties);

    //create trigger properties metadata string
    //nothing for now
    Q_UNUSED(triggerProperties);
}

void CreationDateFieldWizard::loadField(const int fieldId, const int collectionId)
{
    AbstractFieldWizard::loadField(fieldId, collectionId);

    MetadataEngine *meta = &MetadataEngine::getInstance();

    //display properties
    QString displayProperties = meta->getFieldProperties(meta->DisplayProperty,
                                                         fieldId, collectionId);
    MetadataPropertiesParser displayParser(displayProperties);
    if (displayParser.size()) {
        //date format
        int f = displayParser.getValue("dateFormat").toInt();
        if (f)
            ui->dateFormatBox->setCurrentIndex(f-1);
    }
}


//-----------------------------------------------------------------------------
// Private
//-----------------------------------------------------------------------------
