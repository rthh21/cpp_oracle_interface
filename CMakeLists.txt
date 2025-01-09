cmake_minimum_required(VERSION 3.30)
project(main)


# Set the location of Oracle Instant Client
set(OCI_INCLUDE_DIR "C:/Users/rth/Documents/Cod/An2/BazeDeDate/other/sdk/instantclient_23_6/sdk/include")
set(OCI_LIB_DIR "C:/Users/rth/Documents/Cod/An2/BazeDeDate/other/sdk/instantclient_23_6/sdk/lib/msvc")

# Add the Oracle library path to the link directories
link_directories(${OCI_LIB_DIR})
# Specify the source files for your project
set(SOURCES main.cpp baza.cpp)

# Create the executable
add_executable(main ${SOURCES})

# Add include directories for Oracle Instant Client SDK
include_directories(${OCI_INCLUDE_DIR})

# Link the Oracle OCI library (oci.lib)
target_link_libraries(
    main
    oci.lib  # Static Oracle OCI library
    kernel32
    user32
    gdi32
    winspool
    shell32
    ole32
    oleaut32
    uuid
    comdlg32
    advapi32
)

# If you are using dynamic linking and oci.dll, ensure that the Oracle Instant Client DLL is available in your PATH or alongside your executable
