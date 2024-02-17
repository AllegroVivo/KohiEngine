#include "linear_allocator_tests.h"
#include "../test_manager.h"
#include "../expect.h"

#include <defines.h>

#include <memory/linear_allocator.h>

UInt8 linear_allocator_should_create_and_destroy() {
    linear_allocator alloc;
    linear_allocator_create(sizeof(UInt64), 0, &alloc);

    expect_should_not_be(0, alloc.memory);
    expect_should_be(sizeof(UInt64), alloc.total_size);
    expect_should_be(0, alloc.allocated);

    linear_allocator_destroy(&alloc);

    expect_should_be(0, alloc.memory);
    expect_should_be(0, alloc.total_size);
    expect_should_be(0, alloc.allocated);

    return TRUE;
}

UInt8 linear_allocator_single_allocation_all_space() {
    linear_allocator alloc;
    linear_allocator_create(sizeof(UInt64), 0, &alloc);

    void* block = linear_allocator_allocate(&alloc, sizeof(UInt64));

    expect_should_not_be(0, block);
    expect_should_be(sizeof(UInt64), alloc.allocated);

    linear_allocator_destroy(&alloc);

    return TRUE;
}

UInt8 linear_allocator_multi_allocation_all_space() {
    UInt64 max_allocs = 1024;
    linear_allocator alloc;
    linear_allocator_create(sizeof(UInt64) * max_allocs, 0, &alloc);

    void* block;
    for (UInt64 i = 0; i < max_allocs; ++i) {
        block = linear_allocator_allocate(&alloc, sizeof(UInt64));
        expect_should_not_be(0, block);
        expect_should_be(sizeof(UInt64) * (i + 1), alloc.allocated);
    }

    linear_allocator_destroy(&alloc);

    return TRUE;
}

UInt8 linear_allocator_multi_allocation_over_allocate() {
    UInt64 max_allocs = 3;
    linear_allocator alloc;
    linear_allocator_create(sizeof(UInt64) * max_allocs, 0, &alloc);

    void* block;
    for (UInt64 i = 0; i < max_allocs; ++i) {
        block = linear_allocator_allocate(&alloc, sizeof(UInt64));
        expect_should_not_be(0, block);
        expect_should_be(sizeof(UInt64) * (i + 1), alloc.allocated);
    }

    KDEBUG("Note: The following error is intentionally caused by this test.");

    block = linear_allocator_allocate(&alloc, sizeof(UInt64));
    expect_should_be(0, block);
    expect_should_be(sizeof(UInt64) * (max_allocs), alloc.allocated);

    linear_allocator_destroy(&alloc);

    return TRUE;
}

UInt8 linear_allocator_multi_allocation_all_space_then_free() {
    UInt64 max_allocs = 1024;
    linear_allocator alloc;
    linear_allocator_create(sizeof(UInt64) * max_allocs, 0, &alloc);

    void* block;
    for (UInt64 i = 0; i < max_allocs; ++i) {
        block = linear_allocator_allocate(&alloc, sizeof(UInt64));
        expect_should_not_be(0, block);
        expect_should_be(sizeof(UInt64) * (i + 1), alloc.allocated);
    }

    linear_allocator_free_all(&alloc);
    expect_should_be(0, alloc.allocated);

    linear_allocator_destroy(&alloc);

    return TRUE;
}

void linear_allocator_register_tests() {
    test_manager_register_test(linear_allocator_should_create_and_destroy, "Linear allocator should create and destroy");
    test_manager_register_test(linear_allocator_single_allocation_all_space, "Linear allocator single alloc for all space");
    test_manager_register_test(linear_allocator_multi_allocation_all_space, "Linear allocator multi alloc for all space");
    test_manager_register_test(linear_allocator_multi_allocation_over_allocate, "Linear allocator try over allocate");
    test_manager_register_test(linear_allocator_multi_allocation_all_space_then_free, "Linear allocator allocated should be 0 after free_all");
}