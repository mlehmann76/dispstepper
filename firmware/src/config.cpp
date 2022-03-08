#include "config.h"
#include <cassert>
#include <nv_storage.h>

Config::Config() : m_writeRequest(false), m_requestTick(0) {
  nv_storage_init();
  auto s = nv_storage_item_size(ID);
  if (s) {
    auto ret = nv_storage_read(ID, 0x0, reinterpret_cast<uint8_t *>(&m_coll),
                               sizeof(m_coll));
    if(ret != ERR_NONE) {
      resetToDefaults();
    }
  } else {
    // storage has already defaults
  }
}
//
void Config::run(uint32_t tick) {
  if (m_writeRequest) {
    m_requestTick = tick;
    m_writeRequest = false;
  }

  if (m_requestTick != 0 && (uint32_t)(tick - m_requestTick) > CONFIGTIMEOUT) {
    m_requestTick = 0;
    // update storage
    auto ret = nv_storage_write(ID, 0x0, reinterpret_cast<uint8_t *>(&m_coll),
                                sizeof(m_coll));
    assert(ret == ERR_NONE);
  }
}
//
void Config::resetToDefaults() {
  const collection<IDX_Last> def;
  m_coll = def;
  m_requestTick = 0;
  m_writeRequest = false;
  auto ret = nv_storage_write(ID, 0x0, reinterpret_cast<uint8_t *>(&m_coll),
                              sizeof(m_coll));
  if(ret != ERR_NONE) {
    nv_storage_delete(ID);
    auto ret = nv_storage_write(ID, 0x0, reinterpret_cast<uint8_t *>(&m_coll),
                                sizeof(m_coll));
    assert(ret == ERR_NONE);
  }
}
//
bool Config::isDefault() const { 
  bool ret = true;
  const collection<IDX_Last> def;
  for (size_t i=0; i< def.len;i++) {
    if (def.data[i].u32 != m_coll.data[i].u32) {
      ret = false;
      break;
    }
  }
  return ret;
 }
