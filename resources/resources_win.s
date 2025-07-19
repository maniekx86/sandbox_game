// executable embedded resources
//

// button images: buttons.png
    .section .rodata

    .global res_buttons_png
    .balign 4
res_buttons_png:
    .incbin "resources/buttons.png"
res_buttons_png_end:

    .global res_buttons_png_size
    .balign 4
res_buttons_png_size:
    .int res_buttons_png_end - res_buttons_png

// font: joystix.monospace-regular.otf
    .global res_font
    .balign 4
res_font:
    .incbin "resources/joystix.monospace-regular.otf"
res_font_end:

    .global res_font_size
    .balign 4
res_font_size:
    .int res_font_end - res_font

// game logo: logo2.png
    .global res_logo_png
    .balign 4
res_logo_png:
    .incbin "resources/logo2.png"
res_logo_png_end:

    .global res_logo_png_size
    .balign 4
res_logo_png_size:
    .int res_logo_png_end - res_logo_png
