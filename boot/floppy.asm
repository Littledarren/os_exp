INCBIN "../temp/IPL.bin"
INCBIN "../temp/kernel"
;用于生成想要的软盘
times 80 * 18 * 2 * 512 - ($ - $$) db 0
