#include "defuser.h"

#include <inttypes.h>
#include <stdio.h>

#define MAX_FIELDS 8

/*
 * 把 values 中的 count 个整数写成一行“空格分隔的无符号十进制整数”，存入 out。
 * 成功返回 0；count 为 0（还没有实现）、snprintf 出错或结果被截断时返回非零。
 */
static int write_answer(char *out, size_t cap, const uint32_t *values, size_t count) {
    size_t used = 0;
    for (size_t i = 0; i < count; ++i) {
        int size = snprintf(out + used, cap - used, "%s%" PRIu32, i ? " " : "", values[i]);
        if (size < 0 || (size_t)size >= cap - used) return 1;
        used += (size_t)size;
    }
    return count == 0;
}

int solve_phase_1(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 1;
    // TODO: 根据 phase_01 的汇编，用 challenge 算出口令中的各个整数，依次存入 values，并把 count 改为整数个数。
    uint32_t low = (uint32_t)challenge;
    uint32_t high = (uint32_t)(challenge >> 32);
    uint32_t middle = (uint32_t)(challenge >> 16);

    uint32_t step0 =
        (high ^ UINT32_C(0x72bd))
        + UINT32_C(4) * low;

    uint32_t step1 =
        UINT32_C(5) * high
        ^ (middle + UINT32_C(0x4246));

    uint32_t step2 =
        (UINT32_C(5) * step1 + low)
        ^ UINT32_C(0xce9d);

    values[0] = step2 ^ step0;
    return write_answer(out, cap, values, count);
}

int solve_phase_2(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 6;
    // TODO: 根据 phase_02 的汇编，用 challenge 算出口令中的各个整数，依次存入 values，并把 count 改为整数个数。
    values[0] =
        (uint32_t)((challenge >> 6) & UINT64_C(0x7f))
        + UINT32_C(5);

    for (size_t i = 1; i < count; ++i) {
        uint32_t challenge_part =
            (uint32_t)((challenge >> (i + 2)) & UINT64_C(0x3f));

        uint32_t previous_part =
            UINT32_C(5) * values[i - 1];

        values[i] =
            (challenge_part + (uint32_t)i)
            ^ previous_part;
    }
    return write_answer(out, cap, values, count);
}

int solve_phase_3(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 2;
    // TODO: 根据 phase_03 的汇编，用 challenge 算出口令中的各个整数，依次存入 values，并把 count 改为整数个数。
    uint32_t s0 = (uint32_t)challenge & 0x7u;
    uint32_t s1 =
        (uint32_t)(challenge >> 32) ^
        (uint32_t)(challenge >> 3);

    uint32_t result = 0;

    switch (s0) {
    case 0:  /* 跳到 p3_case_5 */
        result = (2u * s1) ^ 0xe4d5u;
        break;

    case 1:  /* 跳到 p3_case_3 */
        result = (s1 ^ 0x67e5u) + (s1 >> 2);
        break;

    case 2:  /* 跳到 p3_case_1 */
        result = (4u * s1) ^ 0xd4efu;
        break;

    case 3:  /* 跳到 p3_case_0 */
        result = 5u * s1 + 0x438fau;
        break;

    case 4:  /* 跳到 p3_case_2 */
        result = 5u * s1 + 0x5d15u;
        break;

    case 5:  /* 跳到 p3_case_4 */
        result = 5u * s1 + 0x9cb0u;
        break;

    case 6:  /* 跳到 p3_case_7 */
        result = 5u * s1 + 0x315f1u;
        break;

    case 7:  /* 跳到 p3_case_6 */
        result = (s1 ^ 0xdadeu) + (s1 >> 2);
        break;

    default:
        return 1;
    }

    values[0] = s0;
    values[1] = result;
    return write_answer(out, cap, values, count);
}


static uint32_t p4_walk_solver(uint32_t target, uint32_t low, uint32_t high,uint32_t key)
// phase4的辅助函数
{
    uint32_t mid = low + ((high - low) >> 1);

    if (target == mid) {
        return (mid + 2u) ^ key;
    }

    uint32_t mix = mid + key;
    uint32_t result;

    if (target < mid) {
        result = p4_walk_solver(target, low, mid - 1u, key);
        return ((5u * result) ^ mix) ^ 0x1eu;
    }

    result = p4_walk_solver(target, mid + 1u, high, key);
    return ((5u * result) ^ mix) ^ 0x13u;
}

int solve_phase_4(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 1;
    // TODO: 根据 phase_04 的汇编，用 challenge 算出口令中的各个整数，依次存入 values，并把 count 改为整数个数。
    uint32_t target = (uint32_t)challenge & 0x7fu;

    uint32_t key =
        (uint32_t)(challenge >> 7) ^
        (uint32_t)(challenge >> 32);

    values[0] = p4_walk_solver(target, 0u, 0x7fu, key);
    return write_answer(out, cap, values, count);
}

int solve_phase_5(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 2;
    // TODO: 根据 phase_05 的汇编，用 challenge 算出口令中的各个整数，依次存入 values，并把 count 改为整数个数。
    static const uint32_t field8[] = {
        793, 1872, 2645, 1119, 2110, 319,
        70, 1292, 597, 2870, 1586, 2316
    };
    static const uint32_t field12[] = {
        3534, 64114, 11036, 50920, 16434, 27533,
        34157, 55659, 52460, 33036, 9192, 35085
    };
    size_t index = (size_t)(challenge & UINT64_C(7));
    uint32_t steps =
        (uint32_t)((challenge >> 3) & UINT64_C(3));
    uint32_t acc =
        (uint32_t)(challenge >> 6)
        ^ (uint32_t)(challenge >> 32)
        ^ UINT32_C(0x9561);
    for (uint32_t i = 0; i <= steps; ++i) {
        acc += field12[index];
        acc *= UINT32_C(5);
        acc ^= field8[index];

        values[1] = field8[index];
        ++index;
    }
    values[0] = acc;

    return write_answer(out, cap, values, count);
}

static uint32_t phase6_apply(uint32_t index, uint32_t x) {
    switch (index) {
        case 0: /* p6_op_3 */
            return (x * UINT32_C(8) + UINT32_C(0x5f1))
                 ^ (x * UINT32_C(5));

        case 1:
        case 7: /* p6_op_2 */
            return ((x >> 7) ^ x) + UINT32_C(0x1d95);

        case 2: /* p6_op_1 */
            return (x + UINT32_C(0xe554)) ^ (x >> 1);

        case 3: /* p6_op_5：32 位循环左移 7 位 */
            return ((x << 7) | (x >> 25)) ^ UINT32_C(0xf72e);

        case 4:
        case 5: /* p6_op_0 */
            return (x ^ UINT32_C(0x866)) + x * UINT32_C(2);

        case 6: /* p6_op_4：32 位循环右移 1 位 */
            return (x >> 1) | (x << 31);

        default:
            return 0;
    }
}
int solve_phase_6(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 1;
    // TODO: 根据 phase_06 的汇编，用 challenge 算出口令中的各个整数，依次存入 values，并把 count 改为整数个数。
    uint32_t x = (uint32_t)(challenge >> 6)
               ^ (uint32_t)(challenge >> 32);

    uint32_t first = (uint32_t)(challenge & UINT64_C(7));
    uint32_t second =
        (uint32_t)((challenge >> 3) & UINT64_C(7));

    x = phase6_apply(first, x);
    x ^= UINT32_C(0x8b0e);
    x = phase6_apply(second, x);

    values[0] = x + UINT32_C(0x4b06);
    return write_answer(out, cap, values, count);
}

int solve_secret(uint64_t challenge, uint64_t context, char *out, size_t cap) {
    // TODO（选做）: 根据 secret_gate、secret_copy 和 secret_phase 的汇编构造完整的第七行，写入 out 后返回 0。
    // 第七行不一定只有整数，可以直接用 snprintf 写入；还没完成时保持返回 1，测试会显示 not attempted。
   uint32_t u = (uint32_t)(challenge >> 32)
               ^ (uint32_t)challenge;

    uint32_t v = (uint32_t)(context >> 32)
               ^ (uint32_t)context;

    uint32_t a = (uint32_t)context & UINT32_C(3);

    uint32_t x = u ^ v;
    uint32_t mix = ((x >> 7) ^ UINT32_C(0x4500))
             + (x ^ UINT32_C(0x720));

    uint32_t b = mix
               ^ ((a * UINT32_C(0x720))
                  ^ (uint32_t)(context >> 16));

    uint32_t c = b
               ^ (v + a + UINT32_C(0x4500) + (u >> 7));

    /* 四组8个A，共32字节；随后是OPEN和三个十进制整数。 */
    int size = snprintf(
        out, cap,
        "AAAAAAAA" "AAAAAAAA" "AAAAAAAA" "AAAAAAAA"
        "OPEN %" PRIu32 " %" PRIu32 " %" PRIu32,
        a, b, c
    );

    return size < 0 || (size_t)size >= cap;
}