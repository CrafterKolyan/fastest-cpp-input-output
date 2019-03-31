#include <iostream>
#include <fstream>
#include <iomanip>
#include <chrono>
#include <cstdio>

#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

using namespace std;

void help(const char *arg0) {
    cout << "Usage: " << arg0 << " <file> [<binary file>]" << endl;
    cout << endl;
    cout << "<file>\tfile to read integers" << endl;
    cout << "<binary file>\tfile with same integers but in binary format" << endl;
}

unsigned int ifstream_input(const char *file_name) {
    ifstream input(file_name);
    unsigned int n;
    input >> n;
    unsigned int x;
    unsigned int ret = 0;
    while (n--) {
        input >> x;
        ret += x;
    }
    return ret;
}

unsigned int fscanf_input(const char *file_name) {
    FILE *input = fopen(file_name, "r");
    unsigned int n;
    fscanf(input, "%u", &n);
    unsigned int x;
    unsigned int ret = 0;
    while (n--) {
        fscanf(input, "%u", &x);
        ret += x;
    }
    fclose(input);
    return ret;
}

unsigned int ifstream_binary_input(const char *file_name) {
    ifstream input(file_name, ifstream::binary);
    unsigned int n;
    input.read((char *)&n, sizeof(n));
    unsigned int x;
    unsigned int ret = 0;
    while (n--) {
        input.read((char *)&x, sizeof(n));
        ret += x;
    }
    return ret;
}

unsigned int fread_binary_input(const char *file_name) {
    FILE *input = fopen(file_name, "rb");
    unsigned int n;
    fread(&n, sizeof(n), 1, input);
    unsigned int x;
    unsigned int ret = 0;
    while (n--) {
        fread(&x, sizeof(x), 1, input);
        ret += x;
    }
    fclose(input);
    return ret;
}

unsigned int fread_buffered_binary_input(const char *file_name) {
    FILE *input = fopen(file_name, "rb");
    unsigned int n;
    fread(&n, sizeof(n), 1, input);
    unsigned int *buffer = (unsigned int *)malloc(n * sizeof(*buffer));
    fread(buffer, sizeof(*buffer), n, input);

    unsigned int ret = 0;
    while (n--) {
        ret += buffer[n];
    }

    fclose(input);
    return ret;
}

unsigned int mmap_input(const char *file_name) {
    int fd_input = open(file_name, O_RDONLY);
    unsigned int *buffer = (unsigned int *)mmap(NULL, sizeof(*buffer), PROT_READ, MAP_SHARED, fd_input, 0);
    unsigned int n = buffer[0];
    munmap(buffer, sizeof(*buffer));
    buffer = (unsigned int *)mmap(NULL, (n + 1) * sizeof(*buffer), PROT_READ, MAP_SHARED, fd_input, 0);

    unsigned int ret = 0;
    while (n--) {
        ret += buffer[n + 1];
    }

    munmap(buffer, sizeof(*buffer));
    return ret;
}

int64_t get_time(unsigned int (*input_function)(const char *), const char *file_name) {
    int64_t start = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    unsigned int ret = input_function(file_name);
    //std::cout << "Sum (to check correctness): " << ret << endl;
    int64_t end = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    return end - start;
}

int main(int argc, char *argv[]) {
    if (argc != 2 && argc != 3) {
        help(argv[0]);
        return 0;
    }
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int64_t time1 = get_time(ifstream_input, argv[1]);
    cout << "ifstream_input\t" << time1 / 1000. / 1000. << " ms" << endl;
    time1 = get_time(fscanf_input, argv[1]);
    cout << "fscanf_input\t" << time1 / 1000. / 1000. << " ms" << endl;
    time1 = get_time(ifstream_binary_input, argv[2]);
    cout << "ifstream_binary_input\t" << time1 / 1000. / 1000. << " ms" << endl;
    time1 = get_time(fread_binary_input, argv[2]);
    cout << "fread_binary_input\t" << time1 / 1000. / 1000. << " ms" << endl;
    time1 = get_time(fread_buffered_binary_input, argv[2]);
    cout << "fread_buffered_binary_input\t" << time1 / 1000. / 1000. << " ms" << endl;
    time1 = get_time(mmap_input, argv[2]);
    cout << "mmap_input\t" << time1 / 1000. / 1000. << " ms" << endl;
}
