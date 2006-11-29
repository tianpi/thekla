#include <Inventor/Qt/SoQt.h>
#include <Inventor/Qt/viewers/SoQtExaminerViewer.h>
#include <Inventor/SoDB.h>
#include <Inventor/nodes/SoSeparator.h>

#include "SoTheklaNode.h"

#include <client/Client.h>

#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoCamera.h>

#include <Inventor/actions/SoWriteAction.h>

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

    InputClient clientIn(Client::QT_APP);
    clientIn.init("SoQt4-Viewer", "localhost", 20000, 20001);

    clientIn.setWriteInterval(100);
//    clientIn.setReadInterval(100);

    {
        OivField * oivCamera = clientIn.insertOivField(&(viewer->getCamera()->position), "cameraPos", true);
        clientIn.bindOivField(oivCamera);
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

    SoWriteAction myAction;
    myAction.getOutput()->openFile("input-scene.iv");
    myAction.getOutput()->setBinary(FALSE);
    myAction.apply(root);
    myAction.getOutput()->closeFile();

    // Clean up resources.
    delete viewer;
    root->unref();

    return 0;
}
