[global memcpy_sse]

memcpy_sse:
  mov edi, [esp+4]
  mov esi, [esp+8]
  mov ebx, [esp+12]

  loop_copy:
    prefetchnta [128+ESI]
    prefetchnta [160+ESI]
    prefetchnta [192+ESI]
    prefetchnta [224+ESI]

    movdqa xmm0, [0+ESI]
    movdqa xmm1, [16+ESI]
    movdqa xmm2, [32+ESI]
    movdqa xmm3, [48+ESI]
    movdqa xmm4, [64+ESI]
    movdqa xmm5, [80+ESI]
    movdqa xmm6, [96+ESI]
    movdqa xmm7, [112+ESI]

    movntdq [0+EDI], xmm0
    movntdq [16+EDI], xmm1
    movntdq [32+EDI], xmm2
    movntdq [48+EDI], xmm3
    movntdq [64+EDI], xmm4
    movntdq [80+EDI], xmm5
    movntdq [96+EDI], xmm6
    movntdq [112+EDI], xmm7

    add esi, 128
    add edi, 128
    dec ebx

    jnz loop_copy
  ret

