#include <Inventor/Qt/SoQt.h>
#include <Inventor/Qt/viewers/SoQtExaminerViewer.h>
#include <Inventor/SoDB.h>
#include <Inventor/nodes/SoSeparator.h>

#include "SoTheklaNode.h"

#include <client/Client.h>

#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoCamera.h>
#include <Inventor/sensors/SoTimerSensor.h>

//--------------------------------------------------------------------------------
static void fieldSensorCB(void * data, SoSensor * sensor)
{
    Q_UNUSED(data);

    SoField * field = (static_cast<SoFieldSensor*>(sensor))->getAttachedField();
    SbString s;
    field->get(s);
    std::cout << "fieldSensorCB(): field.get() = " << s.getString() << std::endl;

    // WARN: You better do not change the field value within a FieldListener, because the
    // FieldListener is called immediately again ;-) -> SoSFBool is inconvenient for that
    // kind of task ?! (-> better use a SoSFTrigger)
//     SoSFBool * fieldBool = dynamic_cast<SoSFBool*>(field);
//     if (fieldBool != NULL) fieldBool->setValue(false);
}

//--------------------------------------------------------------------------------
static void timerSensorCB(void * data, SoSensor * sensor)
{
    Q_UNUSED(sensor);

    // disables event processing on basis of database connection (thus ALL clients which
    // employ the same connection are disabled)
    THEKLA::Client * client = reinterpret_cast<THEKLA::Client *>(data);
    client->enableReading(!client->isReadingEnabled());

    std::cout << "timerSensorCB(): client.isReadingEnabled() = " << client->isReadingEnabled() << std::endl;
}

//--------------------------------------------------------------------------------
int main(int argc, char ** argv)
{
    // Initialize SoQt and Inventor API libraries. This returns a main
    // window to use.
    QWidget * mainwin = SoQt::init(argc, argv, argv[0]);

    // init THEKLA field node
    SoTheklaNode::initClass();

    // Open the argument file..
    SoInput in;
    SbBool ok = in.openFile("cube.iv");
    if (!ok) { exit(1); }

    // ..and import it.
    SoSeparator * root = SoDB::readAll(&in);
    if (root == NULL) { exit(1); }
    root->ref();

    // create minimal scene graph
    SoTheklaNode * node = new SoTheklaNode();
    root->addChild(node);

    SoCube * cube = new SoCube;
    root->addChild(cube);

    // Use the ExaminerViewer, for a nice interface for 3D model
    // inspection.
    SoQtExaminerViewer * viewer = new SoQtExaminerViewer(mainwin);
    viewer->setSceneGraph(root);
    viewer->show();

    //// THEKLA Stuff

    using namespace THEKLA;

    Log log(Log::getClassLog("main-output"));

    OutputClient clientOut(Client::QT_APP);
    clientOut.init("Dialog", "localhost", 20000, 20001);
    clientOut.setReadInterval(100);

    SoSFTrigger touchOnDbChange;
    SoFieldSensor fieldSensor(&fieldSensorCB, NULL);
    fieldSensor.attach(&touchOnDbChange);

    SoTimerSensor timerSensor(&timerSensorCB, &clientOut);
    timerSensor.setInterval(SbTime(1.0));
    timerSensor.schedule();

    //// Client 1: "Dialog"

    {

        QtObject * qtSlider = clientOut.selectQtObject("QSlider", "slider");
        QtProperty * qtSliderValue = clientOut.selectQtProperty(qtSlider, "value", "int");
        clientOut.connectQtPropertyToOivField(qtSliderValue, &(node->value));

        if (!cube->width.connectFrom(&(node->value))) {
            THEKLA_ERROR(log, "connectFrom() failed");
        }

        QtSignal * qtSliderValueChanged = clientOut.selectQtSignal(qtSlider, "valueChanged(int)");
        clientOut.connectQtSignalToOivFieldList(qtSliderValueChanged, &(node->valueChanged));

        if (!cube->height.connectFrom(&(node->valueChangedValue))) {
            THEKLA_ERROR(log, "connectFrom() failed");
        }

        clientOut.connectTouchOnDbChange(&touchOnDbChange);
    }

    //// Client 2: "SoQt4-Viewer"

    OutputClient clientOut2(Client::QT_APP);
    clientOut2.init("SoQt4-Viewer", "localhost", 20000, 20001);
    clientOut2.setReadInterval(100);

    {
        OivField * oivCameraPos = clientOut2.selectOivField("SFVec3f", "cameraPos");
        clientOut2.connectOivFieldToOivField(oivCameraPos, &(viewer->getCamera()->position));
    }


    //// Start the Viewer


    // Pop up the main window.
    SoQt::show(mainwin);
    // Loop until exit.
    try {
        SoQt::mainLoop();
    } catch (std::exception & ex) {
        std::cerr << "++++++++++ <THEKLA-ERROR>" << std::endl;
        std::cerr << ex.what();
        std::cerr << "++++++++++ </THEKLA-ERROR>" << std::endl;
    }

    // Clean up resources.
    delete viewer;
    root->unref();

    return 0;
}
