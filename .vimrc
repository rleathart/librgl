let s:ThisDir = expand('<sfile>:h')

augroup PROJECT_SETTINGS
  autocmd!
  au BufEnter * let b:CMakeBuildDir = PathJoin(s:ThisDir, 'lib')
augroup END
