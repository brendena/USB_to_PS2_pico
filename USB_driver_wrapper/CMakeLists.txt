add_library(USB_driver_wrapper INTERFACE)
target_include_directories(USB_driver_wrapper INTERFACE ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(USB_driver_wrapper INTERFACE tinyusb_board)
target_sources(USB_driver_wrapper INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/hid_keyboard.c
    ${CMAKE_CURRENT_LIST_DIR}/hid_mouse.c
    ${CMAKE_CURRENT_LIST_DIR}/hid_app.c)

    