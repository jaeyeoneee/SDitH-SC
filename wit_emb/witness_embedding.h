#ifndef WITNESS_EMBEDDING_H
#define WITNESS_EMBEDDING_H

#include <stdint.h>

typedef struct {
  uint64_t lambda_bytes;
} wit_emb_params;

void wit_emb_prover_packed_secret_input(
    const wit_emb_params *params,
    uint64_t num_inputs,
    uint8_t *out_pub,
    uint8_t *out_f,
    const uint8_t *in_value,
    const uint8_t *in_rvp_u,
    const uint8_t *in_rvp_v);

#endif
