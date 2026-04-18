set(TESTS
    Test_RectCollision
    Test_RectTouching
    Test_Restrictions
    Test_OverlapSolverManual
    Test_OverlapSolverRandom
    Test_OverlapSolverRandomBig
    Test_OverlapSolverRandomProvider
    Test_ManualGeometryProviderBasic
    Test_LayoutBuilder
    Test_LayoutBuilder2
)

foreach(TEST_NAME ${TESTS})
    add_executable(${TEST_NAME} ${TEST_NAME}.cpp)
    target_link_libraries(${TEST_NAME} PRIVATE troidgen)
endforeach()
