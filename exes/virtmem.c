#include "dmf/type_aliases.h"
#include "dmf/mem.h"


// Page is 16384 bytes

#define PAGE_ALIGN 4096
#define PAGE_SIZE (PAGE_ALIGN * 4)
#define FAN_OUT 64

typedef struct MemPage MemPage;
typedef struct L1 L1;
typedef struct Memory Memory;

struct MemPage {
    u8 page[PAGE_SIZE];
};

struct L1 {
    MemPage **pages;
};

// 
struct Memory {
    L1 *root;
};

int main() {

}