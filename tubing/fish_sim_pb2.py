# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: fish-sim.proto

from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from google.protobuf import reflection as _reflection
from google.protobuf import descriptor_pb2
# @@protoc_insertion_point(imports)




DESCRIPTOR = _descriptor.FileDescriptor(
  name='fish-sim.proto',
  package='',
  serialized_pb='\n\x0e\x66ish-sim.proto\"8\n\x07\x46ishSim\x12\x0f\n\x07\x66ish_id\x18\x01 \x02(\x05\x12\r\n\x05pos_x\x18\x02 \x02(\x02\x12\r\n\x05pos_y\x18\x03 \x02(\x02')




_FISHSIM = _descriptor.Descriptor(
  name='FishSim',
  full_name='FishSim',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='fish_id', full_name='FishSim.fish_id', index=0,
      number=1, type=5, cpp_type=1, label=2,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='pos_x', full_name='FishSim.pos_x', index=1,
      number=2, type=2, cpp_type=6, label=2,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='pos_y', full_name='FishSim.pos_y', index=2,
      number=3, type=2, cpp_type=6, label=2,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  options=None,
  is_extendable=False,
  extension_ranges=[],
  serialized_start=18,
  serialized_end=74,
)

DESCRIPTOR.message_types_by_name['FishSim'] = _FISHSIM

class FishSim(_message.Message):
  __metaclass__ = _reflection.GeneratedProtocolMessageType
  DESCRIPTOR = _FISHSIM

  # @@protoc_insertion_point(class_scope:FishSim)


# @@protoc_insertion_point(module_scope)