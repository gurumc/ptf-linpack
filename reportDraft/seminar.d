# vim: ft=make
.PHONY: seminar._graphics
seminar.aux seminar.aux.make seminar.d seminar.pdf: $(call path-norm,/usr/share/texlive/texmf-dist/tex/generic/oberdiek/etexcmds.sty)
seminar.aux seminar.aux.make seminar.d seminar.pdf: $(call path-norm,/usr/share/texlive/texmf-dist/tex/generic/oberdiek/ifluatex.sty)
seminar.aux seminar.aux.make seminar.d seminar.pdf: $(call path-norm,/usr/share/texlive/texmf-dist/tex/generic/oberdiek/ifpdf.sty)
seminar.aux seminar.aux.make seminar.d seminar.pdf: $(call path-norm,/usr/share/texlive/texmf-dist/tex/generic/oberdiek/infwarerr.sty)
seminar.aux seminar.aux.make seminar.d seminar.pdf: $(call path-norm,/usr/share/texlive/texmf-dist/tex/generic/oberdiek/kvdefinekeys.sty)
seminar.aux seminar.aux.make seminar.d seminar.pdf: $(call path-norm,/usr/share/texlive/texmf-dist/tex/generic/oberdiek/kvsetkeys.sty)
seminar.aux seminar.aux.make seminar.d seminar.pdf: $(call path-norm,/usr/share/texlive/texmf-dist/tex/generic/oberdiek/ltxcmds.sty)
seminar.aux seminar.aux.make seminar.d seminar.pdf: $(call path-norm,/usr/share/texlive/texmf-dist/tex/generic/oberdiek/pdftexcmds.sty)
seminar.aux seminar.aux.make seminar.d seminar.pdf: $(call path-norm,/usr/share/texlive/texmf-dist/tex/latex/appendix/appendix.sty)
seminar.aux seminar.aux.make seminar.d seminar.pdf: $(call path-norm,/usr/share/texlive/texmf-dist/tex/latex/base/article.cls)
seminar.aux seminar.aux.make seminar.d seminar.pdf: $(call path-norm,/usr/share/texlive/texmf-dist/tex/latex/graphics/graphics.sty)
seminar.aux seminar.aux.make seminar.d seminar.pdf: $(call path-norm,/usr/share/texlive/texmf-dist/tex/latex/graphics/graphicx.sty)
seminar.aux seminar.aux.make seminar.d seminar.pdf: $(call path-norm,/usr/share/texlive/texmf-dist/tex/latex/graphics/keyval.sty)
seminar.aux seminar.aux.make seminar.d seminar.pdf: $(call path-norm,/usr/share/texlive/texmf-dist/tex/latex/graphics/trig.sty)
seminar.aux seminar.aux.make seminar.d seminar.pdf: $(call path-norm,/usr/share/texlive/texmf-dist/tex/latex/listings/listings.sty)
seminar.aux seminar.aux.make seminar.d seminar.pdf: $(call path-norm,/usr/share/texlive/texmf-dist/tex/latex/listings/lstmisc.sty)
seminar.aux seminar.aux.make seminar.d seminar.pdf: $(call path-norm,/usr/share/texlive/texmf-dist/tex/latex/oberdiek/epstopdf-base.sty)
seminar.aux seminar.aux.make seminar.d seminar.pdf: $(call path-norm,/usr/share/texlive/texmf-dist/tex/latex/oberdiek/grfext.sty)
seminar.aux seminar.aux.make seminar.d seminar.pdf: $(call path-norm,/usr/share/texlive/texmf-dist/tex/latex/oberdiek/kvoptions.sty)
seminar.aux seminar.aux.make seminar.d seminar.pdf: $(call path-norm,seminar.tex)
.SECONDEXPANSION:
-include ptf_flow.png.gpi.d
seminar.d: $$(call graphics-source,ptf_flow.png,seminar)
seminar.pdf seminar._graphics: $$(call graphics-target,ptf_flow.png,seminar)
