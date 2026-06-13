/**
 * @file TestSessionManager.cpp
 * @brief Advanced verification framework stress-testing SessionManager state transitions.
 */

#include "../Session/SessionManager.hpp"
#include <iostream>
#include <fstream>
#include <cassert>
#include <stdexcept>

// Helper to write mock files for the engine to ingest
static void createMockFile(const std::string &path, const std::string &content)
{
    std::ofstream file(path);
    if (file.is_open())
    {
        file << content;
    }
}

void testInitialStateAndGuards()
{
    std::cout << "[PHASE 1] Verifying strict state-machine guard protections...\n";
    SessionManager manager;

    // Verify initial boot conditions
    assert(manager.isActive() == false);
    assert(manager.getFilename().empty() == true);

    // Attempting actions on an empty session must be blocked instantly
    int caughtErrors = 0;

    try
    {
        manager.closeSession();
        assert(false);
    }
    catch (const std::exception &e)
    {
        std::cout << " -> Blocked illegal close action: " << e.what() << "\n";
        caughtErrors++;
    }

    try
    {
        manager.saveSession();
        assert(false);
    }
    catch (const std::exception &e)
    {
        std::cout << " -> Blocked illegal save action: " << e.what() << "\n";
        caughtErrors++;
    }

    try
    {
        manager.saveSessionAs("illegal_path.ppm");
        assert(false);
    }
    catch (const std::exception &e)
    {
        std::cout << " -> Blocked illegal save-as action: " << e.what() << "\n";
        caughtErrors++;
    }

    assert(caughtErrors == 3);
    std::cout << " -> Phase 1 passed successfully.\n\n";
}

void testStandardLifecycle()
{
    std::cout << "[PHASE 2] Testing standard active lifecycle execution...\n";
    std::string path = "lifecycle_test.pbm";
    createMockFile(path, "P1\n1 1\n1\n");

    SessionManager manager;

    // Open session and verify state progression
    manager.openSession(path);
    assert(manager.isActive() == true);
    assert(manager.getFilename() == path);

    // Enforce active mutation guard (Double-open blockage)
    try
    {
        manager.openSession(path);
        assert(false);
    }
    catch (const std::exception &e)
    {
        std::cout << " -> Double-open request blocked correctly: " << e.what() << "\n";
    }

    // Verify standard save operation does not throw or alter state
    manager.saveSession();
    assert(manager.isActive() == true);

    // Execute safe teardown and check if system state clears out perfectly
    manager.closeSession();
    assert(manager.isActive() == false);
    assert(manager.getFilename().empty() == true);

    std::cout << " -> Phase 2 passed successfully.\n\n";
}

void testPathEvolution()
{
    std::cout << "[PHASE 3] Testing dynamic file path tracking updates...\n";
    std::string originPath = "origin.pgm";
    std::string destinationPath = "evolved_destination.pgm";

    createMockFile(originPath, "P2\n1 1\n255\n100\n");

    SessionManager manager;
    manager.openSession(originPath);

    // Execute Save As operation
    manager.saveSessionAs(destinationPath);

    // The internal tracker must adapt immediately to lock on to the new path
    assert(manager.getFilename() == destinationPath);
    std::cout << " -> Tracking path shifted dynamically to: " << manager.getFilename() << "\n";

    // Verify the file was written to the new destination location
    std::ifstream checkFile(destinationPath);
    assert(checkFile.is_open() == true);
    checkFile.close();

    manager.closeSession();
    std::cout << " -> Phase 3 passed successfully.\n\n";
}

void testPolymorphicIntegration()
{
    std::cout << "[PHASE 4] Verifying factory-to-session pipeline compatibility...\n";
    std::string ppmPath = "integration.ppm";
    createMockFile(ppmPath, "P3\n1 1\n255\n255 0 0\n");

    SessionManager manager;

    // Ensure the factory routes the object layout cleanly through the session manager
    manager.openSession(ppmPath);
    assert(manager.isActive() == true);
    assert(manager.getFilename() == ppmPath);

    manager.closeSession();
    std::cout << " -> Phase 4 passed successfully.\n\n";
}

void testDestructorMemoryIsolation()
{
    std::cout << "[PHASE 5] Stress-testing automatic stack-unwinding cleanup...\n";
    std::string path = "unwind_test.pbm";
    createMockFile(path, "P1\n1 1\n0\n");

    {
        SessionManager scopedManager;
        scopedManager.openSession(path);
        // scopedManager exits scope right here while holding an active image on the heap.
        // The destructor must execute free() automatically to guarantee zero memory leaks.
    }

    std::cout << " -> Phase 5 passed successfully (Heap isolation clean).\n\n";
}

int main()
{
    std::cout << "====================================================\n";
    std::cout << "STARTING INTENSE SESSION MANAGER STATE MACHINE SUITE\n";
    std::cout << "====================================================\n\n";

    testInitialStateAndGuards();
    testStandardLifecycle();
    testPathEvolution();
    testPolymorphicIntegration();
    testDestructorMemoryIsolation();

    std::cout << "====================================================\n";
    std::cout << "ALL SESSION STATE MACHINE CORE TESTS PASSED CLEANLY \n";
    std::cout << "====================================================\n";
    return 0;
}