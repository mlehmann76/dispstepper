#include "config.h"
#include <nv_storage.h>
#include <cassert>

Config::Config() : m_writeRequest(false), m_requestTick(0) {
  nv_storage_init();
  auto s = nv_storage_item_size(ID);
  if (s) {
    auto ret = nv_storage_read(ID, 0x0, reinterpret_cast<uint8_t *>(&m_coll),
                               sizeof(m_coll));
    assert(ret == ERR_NONE);
  } else {
      //TODO initialize storage with defaults
  }
}
void Config::run(uint32_t tick) {
  if (m_writeRequest) {
    m_requestTick = tick;
    m_writeRequest = false;
  }

  if (m_requestTick != 0 && (uint32_t)(tick-m_requestTick) > 1000) {
    m_requestTick = 0;
    //update storage
    //auto ret = nv_storage_write(ID, 0x0, reinterpret_cast<uint8_t *>(&m_coll),
    //                           sizeof(m_coll));
    //assert(ret == ERR_NONE);
  }
}
