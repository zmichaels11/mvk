#pragma once

#include "volk.h"

namespace mvk {
    //! Logic operations used in color logic ops.
    enum class LogicOp : unsigned int {
        CLEAR = VK_LOGIC_OP_CLEAR,
        AND = VK_LOGIC_OP_AND,
        AND_REVERSE = VK_LOGIC_OP_AND_REVERSE,
        COPY = VK_LOGIC_OP_COPY,
        AND_INVERTED = VK_LOGIC_OP_AND_INVERTED,
        NO_OP = VK_LOGIC_OP_NO_OP,
        XOR = VK_LOGIC_OP_XOR,
        OR = VK_LOGIC_OP_OR,
        NOR = VK_LOGIC_OP_NOR,
        EQUIVALENT = VK_LOGIC_OP_EQUIVALENT,
        INVERT = VK_LOGIC_OP_INVERT,
        OR_REVERSE = VK_LOGIC_OP_OR_REVERSE,
        COPY_INVERTE = VK_LOGIC_OP_COPY_INVERTED,
        OR_INVERTED = VK_LOGIC_OP_OR_INVERTED,
        NAND = VK_LOGIC_OP_NAND,
        SET = VK_LOGIC_OP_SET
    };
}
