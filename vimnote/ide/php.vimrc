""""""""""""""""""""""""""""""""
" 当新建php文件时候自动添加php头
"""""""""""""""""""""""""""""""
autocmd BufNewFile *.php exec ":call PhpComment()"
"自动写入注释头信息
func PhpComment()
    call setline(1,"<?php")
    call append(line("."), "/**")
    call append(line(".")+1, " * Description. ")
    call append(line(".")+2, " * ")
    call append(line(".")+3, " * @author Renfeng <renfeng@sogou-inc.com>;")
    call append(line(".")+4, " * @copyright Copyright (C)  2012-".strftime("%Y")." All rights reserved.")
    call append(line(".")+5, " * @version ID")
    call append(line(".")+6, " * @package System")
    call append(line(".")+7, " * Created: ".strftime("%Y-%m-%d %H:%M"))
    call append(line(".")+7, " * Last modified: ".expand("%:t"))
    call append(line(".")+9, " **/")
endfunc

au FileType php setlocal dict+=~/.vim/funclist.txt
if !exists('g:AutoComplPop_Behavior')
        let g:AutoComplPop_Behavior = {}
            let g:AutoComplPop_Behavior['php'] = []
                call add(g:AutoComplPop_Behavior['php'], {
                            \   'command'   : "\<C-x>\<C-o>", 
                            \   'pattern'   : printf('\(->\|::\|\$\)\k\{%d,}$', 0),
                            \   'repeat'    : 0,
                            \})
endif