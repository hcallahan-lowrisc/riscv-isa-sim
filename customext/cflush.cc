#include "insn_macros.h"
#include "extension.h"
#include "decode_macros.h"
#include <cstring>

struct : public arg_t {
  std::string to_string(insn_t insn) const {
    return xpr_name[insn.rs1()];
  }
} xrs1;

static reg_t custom_cflush(processor_t* p, insn_t insn, reg_t pc)
{
  require_privilege(PRV_M);

  return pc + 4; \
}

class cflush_t : public extension_t
{
 public:
  const char* name() const override { return "cflush"; }

  cflush_t() {}

  std::vector<insn_desc_t> get_instructions(const processor_t &) override {
    std::vector<insn_desc_t> insns = {
      {0xFC000073, 0xFFF07FFF, custom_cflush, custom_cflush, custom_cflush, custom_cflush, custom_cflush, custom_cflush, custom_cflush, custom_cflush},
      {0xFC200073, 0xFFF07FFF, custom_cflush, custom_cflush, custom_cflush, custom_cflush, custom_cflush, custom_cflush, custom_cflush, custom_cflush},
      {0xFC100073, 0xFFF07FFF, custom_cflush, custom_cflush, custom_cflush, custom_cflush, custom_cflush, custom_cflush, custom_cflush, custom_cflush}};
    return insns;
  }

  std::vector<disasm_insn_t *> get_disasms(const processor_t *) override {
    std::vector<disasm_insn_t*> insns = {
      new disasm_insn_t("cflush.d.l1", 0xFC000073, 0xFFF07FFF, {&xrs1}),
      new disasm_insn_t("cdiscard.d.l1", 0xFC200073, 0xFFF07FFF, {&xrs1}),
      new disasm_insn_t("cflush.i.l1", 0xFC100073, 0xFFF07FFF, {&xrs1})};
    return insns;
  }
};

REGISTER_EXTENSION(cflush, []() { return new cflush_t; })
