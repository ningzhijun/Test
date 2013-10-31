if &cp | set nocp | endif
let s:cpo_save=&cpo
set cpo&vim
nmap gx <Plug>NetrwBrowseX
nmap wm :WMToggle
nnoremap <silent> <Plug>NetrwBrowseX :call netrw#NetrwBrowseX(expand("<cWORD>"),0)
nnoremap <silent> <F12> :A
nnoremap <silent> <F3> :Grep
let &cpo=s:cpo_save
unlet s:cpo_save
set backspace=indent,eol,start
set completeopt=longest,menu
set fileencodings=utf-8,latin1
set helplang=en
set history=50
set hlsearch
set mouse=v
set pastetoggle=<F11>
set ruler
set shiftwidth=4
set tabstop=4
set textwidth=78
set viminfo='20,\"50
" vim: set ft=vim :
