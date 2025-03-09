/**
 * @file disassembler.hpp
 * @brief Machine code disassembler implementation using Zydis backend
 *
 * @details
 * This header file provides a cross-platform disassembler implementation for
 * x86 and x86-64 architectures using the Zydis disassembly engine. The
 * disassembler is designed to be lightweight, efficient, and easy to use within
 * the mywr memory manipulation framework.
 *
 * The file contains:
 * - Architecture-specific constants for the Zydis backend configuration
 * - An instruction class that encapsulates a disassembled machine code
 * instruction
 * - A disassembler class that provides the core disassembly functionality
 *
 * The implementation supports:
 * - x86 (32-bit) architecture
 * - x86-64 (64-bit) architecture
 * - Instruction metadata extraction (opcode, length, operand count, etc...)
 * - Immediate value extraction (8-bit, 16-bit, 32-bit, 64-bit)
 *
 * Usage example:
 * @code
 * // Create a disassembler instance
 * mywr::disassembler::disassembler disasm;
 *
 * // Disassemble an instruction at a specific address
 * mywr::address code_address = some_function_address;
 * auto instruction = disasm.disassemble(code_address);
 *
 * // Access instruction properties
 * auto length = instruction.length();
 * auto opcode = instruction.opcode();
 *
 * // Extract immediate values if present
 * if (instruction.operand_count() > 0) {
 *     auto imm_value = instruction.imm32();
 * }
 * @endcode
 *
 * @note This implementation is part of the mywr memory manipulation library and
 * relies on the Zydis disassembly engine for the actual disassembly operations.
 *
 * @warning The disassembler assumes that the provided memory addresses are
 * valid and accessible. Attempting to disassemble invalid or inaccessible
 * memory regions may result in undefined behavior or crashes.
 *
 * @see https://github.com/zyantific/zydis for information about the Zydis
 * disassembly engine
 */
#pragma once

#include <shared/core/address.hpp>
#include <shared/core/architecture.hpp>

#if defined(MYWR_X86) || defined(MYWR_X86_64)
  #include <Zydis/Zydis.h>
#endif

#include <array>

/**
 * @brief Internal implementation details for the mywr library.
 */
namespace mywr::detail {
#if defined(MYWR_X86)
/**
 * @brief Machine mode constant for x86 architecture.
 *
 * Defines the machine mode for the Zydis disassembler when targeting 32-bit x86
 * architecture.
 */
constexpr auto kMywrDisassemblerBackendMachineMode =
    ZYDIS_MACHINE_MODE_LONG_COMPAT_32;

/**
 * @brief Stack width constant for x86 architecture.
 *
 * Defines the stack width for the Zydis disassembler when targeting 32-bit x86
 * architecture.
 */
constexpr auto kMywrDisassemblerBackendStackWidth = ZYDIS_STACK_WIDTH_32;
#elif defined(MYWR_X86_64)
/**
 * @brief Machine mode constant for x86-64 architecture.
 *
 * Defines the machine mode for the Zydis disassembler when targeting 64-bit
 * x86-64 architecture.
 */
constexpr auto kMywrDisassemblerBackendMachineMode = ZYDIS_MACHINE_MODE_LONG_64;

/**
 * @brief Stack width constant for x86-64 architecture.
 *
 * Defines the stack width for the Zydis disassembler when targeting 64-bit
 * x86-64 architecture.
 */
constexpr auto kMywrDisassemblerBackendStackWidth = ZYDIS_STACK_WIDTH_64;
#endif

} // namespace mywr::detail

/**
 * @namespace mywr::disassembler
 * @brief Provides functionality for disassembling machine code.
 *
 * This namespace contains classes and utilities for disassembling machine code
 * across different architectures. It currently supports x86 and x86-64
 * architectures using the Zydis disassembler backend.
 */
namespace mywr::disassembler {

/**
 * @class instruction
 * @brief Represents a disassembled machine code instruction.
 *
 * This class encapsulates a decoded instruction and provides methods to access
 * its properties such as opcode, length, operands, and immediate values.
 */
class instruction {
private:
  /** @brief The decoded instruction data structure from Zydis. */
  ZydisDecodedInstruction m_instruction{};

  /** @brief Array of decoded operands for the instruction. */
  std::array<ZydisDecodedOperand, ZYDIS_MAX_OPERAND_COUNT> m_operands{};

  friend class disassembler;

public:
  /**
   * @brief Gets the opcode of the instruction.
   * @return The opcode value.
   */
  [[nodiscard]] auto opcode() const noexcept {
    return m_instruction.opcode;
  }

  /**
   * @brief Gets the length of the instruction in bytes.
   * @return The instruction length.
   */
  [[nodiscard]] auto length() const noexcept {
    return m_instruction.length;
  }

  /**
   * @brief Gets the number of operands in the instruction.
   * @return The operand count.
   */
  [[nodiscard]] auto operand_count() const noexcept {
    return m_instruction.operand_count;
  }

  // NOLINTBEGIN(*-constant-array-index, *-union-access)
  /**
   * @brief Checks if the specified operand is an immediate value.
   *
   * @param operand The index of the operand to check (default: 0).
   * @return true if the operand is an immediate value, false otherwise.
   */
  [[nodiscard]] auto is_immediate_operand(size_t operand = 0) const noexcept {
    return m_operands[operand].type == ZYDIS_OPERAND_TYPE_IMMEDIATE;
  }

  /**
   * @brief Checks if the specified operand is a register.
   *
   * @param operand The index of the operand to check (default: 0).
   * @return true if the operand is a register, false otherwise.
   */
  [[nodiscard]] auto is_register_operand(size_t operand = 0) const noexcept {
    return m_operands[operand].type == ZYDIS_OPERAND_TYPE_REGISTER;
  }

  /**
   * @brief Checks if the specified operand is a memory reference.
   *
   * @param operand The index of the operand to check (default: 0).
   * @return true if the operand is a memory reference, false otherwise.
   */
  [[nodiscard]] auto is_memory_operand(size_t operand = 0) const noexcept {
    return m_operands[operand].type == ZYDIS_OPERAND_TYPE_MEMORY;
  }

  /**
   * @brief Checks if the specified operand is a pointer.
   *
   * @param operand The index of the operand to check (default: 0).
   * @return true if the operand is a pointer, false otherwise.
   */
  [[nodiscard]] auto is_pointer_operand(size_t operand = 0) const noexcept {
    return m_operands[operand].type == ZYDIS_OPERAND_TYPE_POINTER;
  }

  /**
   * @brief Checks if the specified operand is a relative offset.
   *
   * Determines whether the operand is an immediate value that represents
   * a relative offset from the current instruction pointer.
   *
   * @param operand The index of the operand to check (default: 0).
   * @return true if the operand is a relative offset, false otherwise.
   */
  [[nodiscard]] auto is_relative_operand(size_t operand = 0) const noexcept {
    return is_immediate_operand(operand) &&
           (m_operands[operand].imm.is_relative == 1);
  }

  /**
   * @brief Calculates the absolute address for a relative operand.
   *
   * This function calculates the absolute target address for instructions with
   * relative addressing, such as jumps and calls. It uses the Zydis library's
   * address calculation functionality.
   *
   * @param runtime_address The runtime address of the instruction being
   * analyzed.
   * @param operand The index of the operand to use for calculation (default:
   * 0).
   * @return The calculated absolute address as a 64-bit unsigned integer.
   *
   * @note This function is particularly useful for resolving jump targets and
   *       call destinations in disassembled code.
   */
  [[nodiscard]] auto abs(const address& runtime_address,
                         size_t         operand = 0) const noexcept {
    ZyanU64 result{};
    ZydisCalcAbsoluteAddress(&m_instruction,
                             &m_operands[operand],
                             runtime_address.value<ZyanU64>(),
                             &result);
    return result;
  }

  /**
   * @brief Gets the 8-bit immediate value from the specified operand.
   * @param operand The index of the operand (default: 0).
   * @return The 8-bit immediate value.
   */
  [[nodiscard]] auto imm8(size_t operand = 0) const noexcept {
    return static_cast<uint8_t>(m_operands[operand].imm.value.u);
  }

  /**
   * @brief Gets the 16-bit immediate value from the specified operand.
   * @param operand The index of the operand (default: 0).
   * @return The 16-bit immediate value.
   */
  [[nodiscard]] auto imm16(size_t operand = 0) const noexcept {
    return static_cast<uint16_t>(m_operands[operand].imm.value.u);
  }

  /**
   * @brief Gets the 32-bit immediate value from the specified operand.
   * @param operand The index of the operand (default: 0).
   * @return The 32-bit immediate value.
   */
  [[nodiscard]] auto imm32(size_t operand = 0) const noexcept {
    return static_cast<uint32_t>(m_operands[operand].imm.value.u);
  }

  /**
   * @brief Gets the 64-bit immediate value from the specified operand.
   * @param operand The index of the operand (default: 0).
   * @return The 64-bit immediate value.
   */
  [[nodiscard]] auto imm64(size_t operand = 0) const noexcept {
    return static_cast<uint64_t>(m_operands[operand].imm.value.u);
  }
  // NOLINTEND(*-constant-array-index, *-union-access)

private:
  /**
   * @brief Gets a pointer to the internal decoded instruction structure.
   * @return Pointer to the ZydisDecodedInstruction structure.
   */
  [[nodiscard]] auto handle() noexcept -> ZydisDecodedInstruction* {
    return &m_instruction;
  }

  /**
   * @brief Gets a pointer to the array of decoded operands.
   * @return Pointer to the first element of the operands array.
   */
  [[nodiscard]] auto operands() noexcept {
    return m_operands.data();
  }
};

/**
 * @brief Provides functionality to disassemble machine code.
 *
 * This class encapsulates the Zydis decoder and provides methods to disassemble
 * machine code at a given memory address.
 */
class disassembler {
private:
  /** @brief The Zydis decoder instance. */
  ZydisDecoder m_decoder{};

public:
  /**
   * @brief Constructs a disassembler instance.
   *
   * Initializes the Zydis decoder with the appropriate machine mode and stack
   * width based on the target architecture (x86 or x86-64).
   */
  disassembler() noexcept {
    ZydisDecoderInit(&m_decoder,
                     detail::kMywrDisassemblerBackendMachineMode,
                     detail::kMywrDisassemblerBackendStackWidth);
  }

  /**
   * @brief Disassembles a single instruction at the given address.
   *
   * @param code The memory address containing the machine code to disassemble.
   * @return An instruction object representing the disassembled instruction.
   */
  [[nodiscard]] auto disassemble(const address& code) const -> instruction {
    instruction result;

    ZydisDecoderDecodeFull(&m_decoder,
                           code,
                           ZYDIS_MAX_INSTRUCTION_LENGTH,
                           result.handle(),
                           result.operands());

    return result;
  }
};
} // namespace mywr::disassembler
