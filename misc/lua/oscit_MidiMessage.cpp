#include "oscit/midi_message.h"

#include "lua_cpp_helper.h"


using namespace oscit;


/* ============================ Constructors     ====================== */


/** oscit::MidiMessage::MidiMessage()
 * include/oscit/midi_message.h:57
 */
static int MidiMessage_MidiMessage1(lua_State *L) {
  try {
    MidiMessage * retval__ = new MidiMessage();
    lua_pushclass<MidiMessage>(L, retval__, "oscit.MidiMessage");
    return 1;
  } catch (std::exception &e) {
    std::string *s = new std::string("oscit.MidiMessage.MidiMessage: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "oscit.MidiMessage.MidiMessage: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** oscit::MidiMessage::MidiMessage(unsigned int data_size)
 * include/oscit/midi_message.h:61
 */
static int MidiMessage_MidiMessage2(lua_State *L) {
  try {
    int data_size = luaL_checkint(L, 1);
    MidiMessage * retval__ = new MidiMessage(data_size);
    lua_pushclass<MidiMessage>(L, retval__, "oscit.MidiMessage");
    return 1;
  } catch (std::exception &e) {
    std::string *s = new std::string("oscit.MidiMessage.MidiMessage: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "oscit.MidiMessage.MidiMessage: Unknown exception");
    lua_error(L);
    return 0;
  }
}



/** Overloaded function chooser for MidiMessage(...) */
static int MidiMessage_MidiMessage(lua_State *L) {
  int type__ = lua_type(L, 1);
  int top__  = lua_gettop(L);
  if (top__ < 1) {
    return MidiMessage_MidiMessage1(L);
  } else if (type__ == LUA_TNUMBER) {
    return MidiMessage_MidiMessage2(L);
  } else {
    // use any to raise errors
    return MidiMessage_MidiMessage2(L);
  }
}

/* ============================ Destructor       ====================== */

static int MidiMessage_destructor(lua_State *L) {
  MidiMessage **userdata = (MidiMessage**)luaL_checkudata(L, 1, "oscit.MidiMessage");
  if (*userdata) delete *userdata;
  *userdata = NULL;
  return 0;
}

/* ============================ tostring         ====================== */

static int MidiMessage__tostring(lua_State *L) {
  MidiMessage **userdata = (MidiMessage**)luaL_checkudata(L, 1, "oscit.MidiMessage");
  
  lua_pushfstring(L, "<oscit.MidiMessage: %p %s>", *userdata, (*userdata)->to_s().c_str());
  
  return 1;
}

/* ============================ Member Methods   ====================== */


/** static MidiMessage* oscit::MidiMessage::Note(int note, int velocity=80, int length=500, int channel=1, time_t wait=0)
 * include/oscit/midi_message.h:115
 */
static int MidiMessage_Note(lua_State *L) {
  try {
    int top__ = lua_gettop(L);
    MidiMessage * retval__;
    int note = luaL_checkint(L, 1);
    if (top__ < 2) {
      retval__ = MidiMessage::Note(note);
    } else {
      int velocity = luaL_checkint(L, 2);
      if (top__ < 3) {
        retval__ = MidiMessage::Note(note, velocity);
      } else {
        int length = luaL_checkint(L, 3);
        if (top__ < 4) {
          retval__ = MidiMessage::Note(note, velocity, length);
        } else {
          int channel = luaL_checkint(L, 4);
          if (top__ < 5) {
            retval__ = MidiMessage::Note(note, velocity, length, channel);
          } else {
            time_t wait = luaL_checknumber(L, 5);
            retval__ = MidiMessage::Note(note, velocity, length, channel, wait);
          }
        }
      }
    }
    lua_pushclass<MidiMessage>(L, retval__, "oscit.MidiMessage");
    return 1;
  } catch (std::exception &e) {
    std::string *s = new std::string("oscit.MidiMessage.Note: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "oscit.MidiMessage.Note: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** unsigned int oscit::MidiMessage::channel() const 
 * include/oscit/midi_message.h:229
 */
static int MidiMessage_channel(lua_State *L) {
  try {
    MidiMessage *self__ = *((MidiMessage**)luaL_checkudata(L, 1, "oscit.MidiMessage"));
    lua_remove(L, 1);
    int  retval__ = self__->channel();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    std::string *s = new std::string("oscit.MidiMessage.channel: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "oscit.MidiMessage.channel: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** unsigned int oscit::MidiMessage::ctrl() const 
 * include/oscit/midi_message.h:225
 */
static int MidiMessage_ctrl(lua_State *L) {
  try {
    MidiMessage *self__ = *((MidiMessage**)luaL_checkudata(L, 1, "oscit.MidiMessage"));
    lua_remove(L, 1);
    int  retval__ = self__->ctrl();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    std::string *s = new std::string("oscit.MidiMessage.ctrl: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "oscit.MidiMessage.ctrl: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** time_t oscit::MidiMessage::length() const 
 * include/oscit/midi_message.h:244
 */
static int MidiMessage_length(lua_State *L) {
  try {
    MidiMessage *self__ = *((MidiMessage**)luaL_checkudata(L, 1, "oscit.MidiMessage"));
    lua_remove(L, 1);
    time_t  retval__ = self__->length();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    std::string *s = new std::string("oscit.MidiMessage.length: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "oscit.MidiMessage.length: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** unsigned int oscit::MidiMessage::note() const 
 * include/oscit/midi_message.h:223
 */
static int MidiMessage_note(lua_State *L) {
  try {
    MidiMessage *self__ = *((MidiMessage**)luaL_checkudata(L, 1, "oscit.MidiMessage"));
    lua_remove(L, 1);
    int  retval__ = self__->note();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    std::string *s = new std::string("oscit.MidiMessage.note: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "oscit.MidiMessage.note: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** void oscit::MidiMessage::note_on_to_off()
 * include/oscit/midi_message.h:152
 */
static int MidiMessage_note_on_to_off(lua_State *L) {
  try {
    MidiMessage *self__ = *((MidiMessage**)luaL_checkudata(L, 1, "oscit.MidiMessage"));
    lua_remove(L, 1);
    self__->note_on_to_off();
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("oscit.MidiMessage.note_on_to_off: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "oscit.MidiMessage.note_on_to_off: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** void oscit::MidiMessage::set_as_ctrl(int ctrl, int ctrl_value, unsigned int channel=1, time_t wait=0)
 * include/oscit/midi_message.h:144
 */
static int MidiMessage_set_as_ctrl(lua_State *L) {
  try {
    MidiMessage *self__ = *((MidiMessage**)luaL_checkudata(L, 1, "oscit.MidiMessage"));
    lua_remove(L, 1);
    int top__ = lua_gettop(L);
    int ctrl = luaL_checkint(L, 1);
    int ctrl_value = luaL_checkint(L, 2);
    if (top__ < 3) {
      self__->set_as_ctrl(ctrl, ctrl_value);
    } else {
      int channel = luaL_checkint(L, 3);
      if (top__ < 4) {
        self__->set_as_ctrl(ctrl, ctrl_value, channel);
      } else {
        time_t wait = luaL_checknumber(L, 4);
        self__->set_as_ctrl(ctrl, ctrl_value, channel, wait);
      }
    }
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("oscit.MidiMessage.set_as_ctrl: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "oscit.MidiMessage.set_as_ctrl: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** void oscit::MidiMessage::set_as_note(int note, int velocity=80, int length=500, int channel=1, time_t wait=0)
 * include/oscit/midi_message.h:130
 */
static int MidiMessage_set_as_note(lua_State *L) {
  try {
    MidiMessage *self__ = *((MidiMessage**)luaL_checkudata(L, 1, "oscit.MidiMessage"));
    lua_remove(L, 1);
    int top__ = lua_gettop(L);
    int note = luaL_checkint(L, 1);
    if (top__ < 2) {
      self__->set_as_note(note);
    } else {
      int velocity = luaL_checkint(L, 2);
      if (top__ < 3) {
        self__->set_as_note(note, velocity);
      } else {
        int length = luaL_checkint(L, 3);
        if (top__ < 4) {
          self__->set_as_note(note, velocity, length);
        } else {
          int channel = luaL_checkint(L, 4);
          if (top__ < 5) {
            self__->set_as_note(note, velocity, length, channel);
          } else {
            time_t wait = luaL_checknumber(L, 5);
            self__->set_as_note(note, velocity, length, channel, wait);
          }
        }
      }
    }
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("oscit.MidiMessage.set_as_note: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "oscit.MidiMessage.set_as_note: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** void oscit::MidiMessage::set_channel(int channel)
 * include/oscit/midi_message.h:181
 */
static int MidiMessage_set_channel(lua_State *L) {
  try {
    MidiMessage *self__ = *((MidiMessage**)luaL_checkudata(L, 1, "oscit.MidiMessage"));
    lua_remove(L, 1);
    int channel = luaL_checkint(L, 1);
    self__->set_channel(channel);
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("oscit.MidiMessage.set_channel: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "oscit.MidiMessage.set_channel: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** void oscit::MidiMessage::set_ctrl(int ctrl)
 * include/oscit/midi_message.h:167
 */
static int MidiMessage_set_ctrl(lua_State *L) {
  try {
    MidiMessage *self__ = *((MidiMessage**)luaL_checkudata(L, 1, "oscit.MidiMessage"));
    lua_remove(L, 1);
    int ctrl = luaL_checkint(L, 1);
    self__->set_ctrl(ctrl);
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("oscit.MidiMessage.set_ctrl: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "oscit.MidiMessage.set_ctrl: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** void oscit::MidiMessage::set_key(int note)
 * include/oscit/midi_message.h:171
 */
static int MidiMessage_set_key(lua_State *L) {
  try {
    MidiMessage *self__ = *((MidiMessage**)luaL_checkudata(L, 1, "oscit.MidiMessage"));
    lua_remove(L, 1);
    int note = luaL_checkint(L, 1);
    self__->set_key(note);
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("oscit.MidiMessage.set_key: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "oscit.MidiMessage.set_key: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** void oscit::MidiMessage::set_length(int length)
 * include/oscit/midi_message.h:203
 */
static int MidiMessage_set_length(lua_State *L) {
  try {
    MidiMessage *self__ = *((MidiMessage**)luaL_checkudata(L, 1, "oscit.MidiMessage"));
    lua_remove(L, 1);
    int length = luaL_checkint(L, 1);
    self__->set_length(length);
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("oscit.MidiMessage.set_length: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "oscit.MidiMessage.set_length: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** void oscit::MidiMessage::set_note(int note)
 * include/oscit/midi_message.h:163
 */
static int MidiMessage_set_note(lua_State *L) {
  try {
    MidiMessage *self__ = *((MidiMessage**)luaL_checkudata(L, 1, "oscit.MidiMessage"));
    lua_remove(L, 1);
    int note = luaL_checkint(L, 1);
    self__->set_note(note);
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("oscit.MidiMessage.set_note: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "oscit.MidiMessage.set_note: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** void oscit::MidiMessage::set_type(MidiMessageType type)
 * include/oscit/midi_message.h:159
 */
static int MidiMessage_set_type(lua_State *L) {
  try {
    MidiMessage *self__ = *((MidiMessage**)luaL_checkudata(L, 1, "oscit.MidiMessage"));
    lua_remove(L, 1);
    MidiMessageType *type = *((MidiMessageType **)luaL_checkudata(L, 1, "oscit.MidiMessageType"));
    self__->set_type(*type);
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("oscit.MidiMessage.set_type: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "oscit.MidiMessage.set_type: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** void oscit::MidiMessage::set_value(int value)
 * include/oscit/midi_message.h:211
 */
static int MidiMessage_set_value(lua_State *L) {
  try {
    MidiMessage *self__ = *((MidiMessage**)luaL_checkudata(L, 1, "oscit.MidiMessage"));
    lua_remove(L, 1);
    int value = luaL_checkint(L, 1);
    self__->set_value(value);
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("oscit.MidiMessage.set_value: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "oscit.MidiMessage.set_value: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** void oscit::MidiMessage::set_velocity(int velocity)
 * include/oscit/midi_message.h:199
 */
static int MidiMessage_set_velocity(lua_State *L) {
  try {
    MidiMessage *self__ = *((MidiMessage**)luaL_checkudata(L, 1, "oscit.MidiMessage"));
    lua_remove(L, 1);
    int velocity = luaL_checkint(L, 1);
    self__->set_velocity(velocity);
    return 0;
  } catch (std::exception &e) {
    std::string *s = new std::string("oscit.MidiMessage.set_velocity: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "oscit.MidiMessage.set_velocity: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** MidiMessageType oscit::MidiMessage::type() const 
 * include/oscit/midi_message.h:221
 */
static int MidiMessage_type(lua_State *L) {
  try {
    MidiMessage *self__ = *((MidiMessage**)luaL_checkudata(L, 1, "oscit.MidiMessage"));
    lua_remove(L, 1);
    MidiMessageType  retval__ = self__->type();
    lua_pushclass<MidiMessageType>(L, retval__, "oscit.MidiMessageType");
    return 1;
  } catch (std::exception &e) {
    std::string *s = new std::string("oscit.MidiMessage.type: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "oscit.MidiMessage.type: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** unsigned int oscit::MidiMessage::value() const 
 * include/oscit/midi_message.h:227
 */
static int MidiMessage_value(lua_State *L) {
  try {
    MidiMessage *self__ = *((MidiMessage**)luaL_checkudata(L, 1, "oscit.MidiMessage"));
    lua_remove(L, 1);
    int  retval__ = self__->value();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    std::string *s = new std::string("oscit.MidiMessage.value: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "oscit.MidiMessage.value: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** unsigned int oscit::MidiMessage::velocity() const 
 * include/oscit/midi_message.h:242
 */
static int MidiMessage_velocity(lua_State *L) {
  try {
    MidiMessage *self__ = *((MidiMessage**)luaL_checkudata(L, 1, "oscit.MidiMessage"));
    lua_remove(L, 1);
    int  retval__ = self__->velocity();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    std::string *s = new std::string("oscit.MidiMessage.velocity: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "oscit.MidiMessage.velocity: Unknown exception");
    lua_error(L);
    return 0;
  }
}


/** time_t oscit::MidiMessage::wait() const 
 * include/oscit/midi_message.h:246
 */
static int MidiMessage_wait(lua_State *L) {
  try {
    MidiMessage *self__ = *((MidiMessage**)luaL_checkudata(L, 1, "oscit.MidiMessage"));
    lua_remove(L, 1);
    time_t  retval__ = self__->wait();
    lua_pushnumber(L, retval__);
    return 1;
  } catch (std::exception &e) {
    std::string *s = new std::string("oscit.MidiMessage.wait: ");
    s->append(e.what());
    lua_pushstring(L, s->c_str());
    delete s;
    lua_error(L);
    // never reached
    return 0;
  } catch (...) {
    lua_pushstring(L, "oscit.MidiMessage.wait: Unknown exception");
    lua_error(L);
    return 0;
  }
}




/* ============================ Lua Registration ====================== */

static const struct luaL_Reg MidiMessage_member_methods[] = {
  {"channel"           , MidiMessage_channel},
  {"ctrl"              , MidiMessage_ctrl},
  {"length"            , MidiMessage_length},
  {"note"              , MidiMessage_note},
  {"note_on_to_off"    , MidiMessage_note_on_to_off},
  {"set_as_ctrl"       , MidiMessage_set_as_ctrl},
  {"set_as_note"       , MidiMessage_set_as_note},
  {"set_channel"       , MidiMessage_set_channel},
  {"set_ctrl"          , MidiMessage_set_ctrl},
  {"set_key"           , MidiMessage_set_key},
  {"set_length"        , MidiMessage_set_length},
  {"set_note"          , MidiMessage_set_note},
  {"set_type"          , MidiMessage_set_type},
  {"set_value"         , MidiMessage_set_value},
  {"set_velocity"      , MidiMessage_set_velocity},
  {"type"              , MidiMessage_type},
  {"value"             , MidiMessage_value},
  {"velocity"          , MidiMessage_velocity},
  {"wait"              , MidiMessage_wait},
  {"__tostring"        , MidiMessage__tostring},
  {"__gc"              , MidiMessage_destructor},
  {NULL, NULL},
};

static const struct luaL_Reg MidiMessage_namespace_methods[] = {
  {"MidiMessage"       , MidiMessage_MidiMessage},
  {"MidiMessage_Note"  , MidiMessage_Note},
  {NULL, NULL},
};



#ifdef DUB_LUA_NO_OPEN
int luaload_oscit_MidiMessage(lua_State *L) {
#else
extern "C" int luaopen_oscit_MidiMessage(lua_State *L) {
#endif
  // Create the metatable which will contain all the member methods
  luaL_newmetatable(L, "oscit.MidiMessage");

  // metatable.__index = metatable (find methods in the table itself)
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");

  // register member methods
  luaL_register(L, NULL, MidiMessage_member_methods);

  // register class methods in a global namespace table
  luaL_register(L, "oscit", MidiMessage_namespace_methods);


}
