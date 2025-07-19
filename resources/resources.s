// executable embeded resources
//

// fixes "missing .note.GNU-stack section implies executable stack" warning
.section .note.GNU-stack,"",@progbits 

// button images: buttons.png
    .section .rodata

    .global res_buttons_png
    .type   res_buttons_png, @object
    .balign 4
res_buttons_png:
    .incbin "resources/buttons.png"
res_buttons_png_end:

    .global res_buttons_png_size
    .type   res_buttons_png_size, @object
    .balign 4
res_buttons_png_size:
    .int    res_buttons_png_end - res_buttons_png

// font: joystix.monospace-regular.otf
    .global res_font
    .type   res_font, @object
    .balign 4
res_font:
    .incbin "resources/joystix.monospace-regular.otf"
res_font_end:

    .global res_font_size
    .type   res_font_size, @object
    .balign 4
res_font_size:
    .int    res_font_end - res_font

// game logo: logo2.png
    .global res_logo_png
    .type   res_logo_png, @object
    .balign 4
res_logo_png:
    .incbin "resources/logo2.png"
res_logo_png_end:

    .global res_logo_png_size
    .type   res_logo_png_size, @object
    .balign 4
res_logo_png_size:
    .int    res_logo_png_end - res_logo_png
