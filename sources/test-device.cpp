#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/mman.h>

typedef uint32_t* uint32_p;

struct file_s {
    int file;
    int error;
    bool file_open(const char* name, int flags) {
        file = ::open(name, flags);
        error = errno;
        return (0 <= file);
    }
    bool file_close() {
        if (file < 0) {
            return true;
        }
        int v = ::close(file);
        error = errno;
        file = -1;
        return (0 == v);
    }
    bool file_size(size_t& size) {
        size = 0;
        off_t offset = ::lseek(file, 0, SEEK_END);
        error = errno;
        printf("lseek(END) returns %ld error %d\n", (long)offset, error);
        if (offset < 0) {
            return false;    
        }
        size = offset;
        return true;
        // off_t zero = ::lseek(file, 0, SEEK_SET);
        // error = errno;
        // return (0 == zero);
    }
    file_s() : file(-1), error(0) {
    }
    virtual ~file_s() {
        file_close();
    }
};

static int prot_to_open_flags(int prot) {
    switch ((PROT_READ|PROT_WRITE) & prot) {
    case PROT_READ|PROT_WRITE:
        return O_RDWR;
    case PROT_READ:
        return O_RDONLY;
    case PROT_WRITE:
        return O_WRONLY;
    }
    return O_RDWR;
}

struct file_mmap_s {
    size_t map_size;
    uint32_p map_base;
    int error;
    bool mmap_open(const char* name, int prot);
    bool mmap_close();
    file_mmap_s() : map_size(0), map_base((uint32_p)MAP_FAILED) {
    }
    virtual ~file_mmap_s() {
        mmap_close();
    }
};

bool file_mmap_s::mmap_open(const char* name, int prot) {
    file_s file;
    int oflags = prot_to_open_flags(prot);
    if (!file.file_open(name, oflags)) {
        error = file.error;
        printf("ERROR cannot open file! name: %s error: %d\n", name, error);
        return false;
    }
    if (!file.file_size(map_size)) {
        error = file.error;
        printf("ERROR size file! name: %s error: %d\n", name, error);
        return false;
    }
    map_base = (uint32_p) ::mmap(
        0,      // Let system place in memory.
        map_size,
        prot,
        MAP_SHARED,
        file.file,
        0       // Map from offset 0 in file.
    );
    error = errno;
    if (MAP_FAILED == map_base) {
        printf("ERROR mmap() failed - errno: %d\n", error);
        return false;
    }
    return true;
}

bool file_mmap_s::mmap_close() {
    if (MAP_FAILED == map_base) {
        return true;
    }
    int v = ::munmap(map_base, map_size);
    error = errno;
    map_base = (uint32_p)MAP_FAILED;
    map_size = 0;
    return (0 == v);
}

static struct options_s {
    int verbose;
} g_options = {
    0
};

void print_maps(const char* note) {
    int pid = getpid();
    char command[80];
    ::sprintf(command, "cat /proc/%d/maps", pid);
    printf("\nProcess maps -- %s\ncommand: %s\n", note, command);
    system(command);
    printf("=====\n");
}

void print_buffer(file_mmap_s& o, unsigned n, const char* name) {
    for (unsigned i = 0; i < n; ++i) {
        printf("%04x: %08x -- %s\n", i, o.map_base[i], name);
    }
}

bool test_device() {
    file_s file_control;
    if (!file_control.file_open("/dev/exampleA/control", O_RDWR|O_CLOEXEC)) {
        printf("ERROR cannot open control file! error: %d\n", file_control.error);
        return false;
    }
    file_mmap_s mmap_request;
    if (!mmap_request.mmap_open("/dev/exampleA/request", PROT_READ|PROT_WRITE)) {
        printf("ERROR cannot mmap request file! error: %d\n", mmap_request.error);
        return false;
    }
    file_mmap_s mmap_response;
    if (!mmap_response.mmap_open("/dev/exampleA/response", PROT_READ)) {
        printf("ERROR cannot mmap response file! error: %d\n", mmap_response.error);
        return false;
    }
    file_mmap_s mmap_status;
    if (!mmap_status.mmap_open("/dev/exampleA/status", PROT_READ)) {
        printf("ERROR cannot mmap status file! error: %d\n", mmap_status.error);
        return false;
    }
    file_mmap_s mmap_antenna1;
    if (!mmap_antenna1.mmap_open("/dev/exampleA/antenna1", PROT_READ)) {
        printf("ERROR cannot mmap antenna1 file! error: %d\n", mmap_antenna1.error);
        return false;
    }
    file_mmap_s mmap_antenna2;
    if (!mmap_antenna2.mmap_open("/dev/exampleA/antenna2", PROT_READ)) {
        printf("ERROR cannot mmap antenna2 file! error: %d\n", mmap_antenna2.error);
        return false;
    }
    file_mmap_s mmap_antenna3;
    if (!mmap_antenna3.mmap_open("/dev/exampleA/antenna3", PROT_READ)) {
        printf("ERROR cannot mmap antenna3 file! error: %d\n", mmap_antenna3.error);
        return false;
    }
    file_mmap_s mmap_antenna4;
    if (!mmap_antenna4.mmap_open("/dev/exampleA/antenna4", PROT_READ)) {
        printf("ERROR cannot mmap antenna4 file! error: %d\n", mmap_antenna4.error);
        return false;
    }
    print_maps("with mapped files");
    // Report version from status.
    printf("status.version = %08x\n", mmap_status.map_base[0]);
    print_buffer(mmap_request, 10, "request");
    print_buffer(mmap_response, 10, "response");
    print_buffer(mmap_status, 10, "status");
    print_buffer(mmap_antenna1, 10, "antenna1");
    print_buffer(mmap_antenna2, 10, "antenna2");
    print_buffer(mmap_antenna3, 10, "antenna3");
    print_buffer(mmap_antenna4, 10, "antenna4");
    return true;
}

void usage() {
    exit(1);
}

bool options_set(int ac, char** av) {
    for (;;) {
        int c = ::getopt(ac, av,"hv");
        if (c < 0) {
            break;
        }
        switch (c) {
        case 'v':
            g_options.verbose++;
            break;
        default:
            usage();
        }
    }
    return true;
}

int main(int ac, char** av) {
    printf("ac %d, av %lx\n", ac, (long)av);
    if (!options_set(ac, av)) {
        return 1;
    }
    test_device();
    return 0;
}
