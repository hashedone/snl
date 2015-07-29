#!python

import os

env = Environment(
    tools=['default', 'protoc'],
    toolpath=['#tools'],
    CXXFLAGS=['--std=c++14'],
    CPPPATH=['#build'],
    LIBS=['pthread', 'boost_system', 'curses', 'protobuf']
)

proto = Glob('common/Network/Types/proto/*.proto')

proto_targets = env.Protoc(
    proto,
    PROTOC_PATH='common/Network/Types/proto',
    PROTOC_CCOUT='common/Network/Types'
)

env.Alias('proto', proto_targets)

sources = [File('main.cpp')]
sources += Glob('client/*.cpp')
sources += Glob('gui/*.cpp')
sources += Glob('server/*.cpp')
sources += [f for f in proto_targets if os.path.splitext(f.name)[1] == '.cc']

env.Program('snl', sources)

