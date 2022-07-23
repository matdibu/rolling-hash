enable_testing()

set(SuccessTests 
)

set(FailTests 
)

foreach(test ${SuccessTests})
    add_test(NAME ${test} COMMAND ${PROJECT_NAME} ../tests/${test}/old ../tests/${test}/new ../tests/${test}/output)
endforeach()

foreach(test ${FailTests})
    add_test(NAME ${test} COMMAND ${PROJECT_NAME} ../tests/${test}/old ../tests/${test}/new ../tests/${test}/output)
    set_tests_properties(${test} PROPERTIES WILL_FAIL True)
endforeach()

