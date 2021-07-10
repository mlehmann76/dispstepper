#include "config.h"
#include <nv_storage.h>

Config::Config() {
  nv_storage_init();
  auto s = nv_storage_item_size(ID);
  if (s) {
    auto ret = nv_storage_read(ID, 0x0, reinterpret_cast<uint8_t *>(&m_coll),
                               sizeof(m_coll));
  } else {
      //TODO initialize storage with defaults
  }
}
