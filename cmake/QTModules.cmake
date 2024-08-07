# List of all Qt modules
set(MODULES)

set(QT_DEFAULT_MAJOR_VERSION 6)

set(QT_COMPONENTS Core Gui Widgets Network Sql StateMachine)

if (ENABLE_TESTS)
	list(APPEND QT_COMPONENTS Test)
endif()