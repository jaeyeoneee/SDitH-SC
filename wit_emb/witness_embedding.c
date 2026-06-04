#include "witness_embedding.h"

#include <string.h>

static void bitvec_xor(uint64_t nbytes, uint8_t *out, const uint8_t *a, const uint8_t *b) {
  for (uint64_t i = 0; i < nbytes; ++i) {
    out[i] = a[i] ^ b[i];
  }
}

static void flambda_set(uint64_t lambda_bytes, uint8_t *out, const uint8_t *in) {
  memcpy(out, in, lambda_bytes);
}

/*
 * Standalone extraction of:
 *   prover_cst_vole_packed_secret_input_ct_ref
 *
 * from:
 *   SDitH-v2/Reference_Implementation/sdith_cat1_fast/src/piop_circuit.c
 *
 * It embeds packed witness bits into the VOLE/PIOP input representation:
 *
 *   out_pub = in_rvp_u XOR in_value
 *   out_f[i] = constant term w_i || X coefficient V_i
 *
 * where each out_f[i] occupies 2 * lambda_bytes bytes.
 */
void wit_emb_prover_packed_secret_input(
    const wit_emb_params *params,
    uint64_t num_inputs,
    uint8_t *out_pub,
    uint8_t *out_f,
    const uint8_t *in_value,
    const uint8_t *in_rvp_u,
    const uint8_t *in_rvp_v) {
  const uint64_t lambda_bytes = params->lambda_bytes;
  uint8_t *const f = out_f;
  const uint8_t *const rvp_v = in_rvp_v;
  const uint8_t *const inval = in_value;
  const uint64_t input_bytes = (num_inputs + 7) >> 3;

  bitvec_xor(input_bytes, out_pub, in_rvp_u, in_value);

  uint64_t leftover_bits = num_inputs & 7;
  if (leftover_bits) {
    uint64_t idx = input_bytes - 1;
    uint8_t and_mask = (uint8_t)((1u << leftover_bits) - 1u);
    out_pub[idx] &= and_mask;
  }

  memset(out_f, 0, 2 * num_inputs * lambda_bytes);
  for (uint64_t i = 0; i < num_inputs; i++) {
    uint8_t *const fi = f + i * 2 * lambda_bytes;
    fi[0] = (uint8_t)((inval[i >> 3] >> (i & 7)) & 1u);
    flambda_set(lambda_bytes, fi + lambda_bytes, rvp_v + i * lambda_bytes);
  }
}
