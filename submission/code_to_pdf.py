# AI-generated submission tool (not part of the C++ project).
"""
code_to_pdf.py — concatenate .h/.cpp files into a single UTF-8-safe PDF
with a real text layer (selectable/searchable, unlike paps; correct
apostrophes and box-drawing glyphs, unlike enscript).

Usage:
    pip install fpdf2
    python3 code_to_pdf.py <src_dir> <output.pdf> [font.ttf]
    python3 code_to_pdf.py ./src/ ./submission/code.pdf DejaVuSansMono.ttf
    uv run code_to_pdf.py ../src/ ../submission/code.pdf DejaVuSansMono.ttf

macOS font suggestion: /System/Library/Fonts/Menlo.ttc won't work (ttc);
use DejaVuSansMono.ttf (bundled with many tools, or download from
https://dejavu-fonts.github.io) or any monospace .ttf.
"""
import sys
from pathlib import Path
from fpdf import FPDF

FONT_SIZE = 8
LINE_H = 3.6          # mm per line
MARGIN = 12           # mm

def find_font():
    """Locate DejaVuSansMono.ttf without a manual download if possible."""
    candidates = [
        Path.cwd() / "DejaVuSansMono.ttf",
        Path.home() / "Library/Fonts/DejaVuSansMono.ttf",             # macOS user fonts
        Path("/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf"),  # Linux
    ]
    try:  # matplotlib bundles the DejaVu fonts
        import matplotlib
        candidates.insert(0, Path(matplotlib.get_data_path()) / "fonts/ttf/DejaVuSansMono.ttf")
    except ImportError:
        pass
    for c in candidates:
        if c.exists():
            return str(c)
    sys.exit("No monospace TTF found. Easiest fix:  pip install matplotlib\n"
             "or put DejaVuSansMono.ttf next to this script, or pass a .ttf path as arg 3.")

def main():
    if len(sys.argv) < 3:
        print(__doc__)
        sys.exit(1)
    src_dir = Path(sys.argv[1])
    out_pdf = sys.argv[2]
    font_path = sys.argv[3] if len(sys.argv) > 3 else find_font()

    # headers first, then implementations — same ordering as the shell script
    files = sorted(src_dir.glob("*.h")) + sorted(src_dir.glob("*.cpp"))
    if not files:
        print(f"No .h/.cpp files found in {src_dir}")
        sys.exit(1)

    pdf = FPDF(orientation="P", unit="mm", format="A4")
    pdf.set_margins(MARGIN, MARGIN, MARGIN)
    pdf.set_auto_page_break(True, margin=MARGIN)
    pdf.add_font("mono", fname=font_path)
    pdf.set_font("mono", size=FONT_SIZE)

    usable_w = pdf.w - 2 * MARGIN
    # max chars per line for this font size (measure a wide char)
    char_w = pdf.get_string_width("M")
    max_chars = int(usable_w / char_w)

    for f in files:
        pdf.add_page()                       # each file starts on a new page
        # --- file banner ---
        bar = "=" * 60
        title = f.name.center(56)
        pdf.set_font("mono", size=FONT_SIZE + 2)
        for banner_line in (f"// {bar}", f"// =={title}==", f"// {bar}", ""):
            pdf.cell(0, LINE_H + 1, banner_line, new_x="LMARGIN", new_y="NEXT")
        pdf.set_font("mono", size=FONT_SIZE)

        text = f.read_text(encoding="utf-8", errors="replace")
        for raw_line in text.splitlines():
            line = raw_line.replace("\t", "    ")
            if not line:
                pdf.cell(0, LINE_H, "", new_x="LMARGIN", new_y="NEXT")
                continue
            # hard-wrap long lines with a visible continuation marker
            first = True
            while line:
                chunk, line = line[:max_chars], line[max_chars:]
                prefix = "" if first else "\u21aa "   # ↪ marks wrapped continuation
                pdf.cell(0, LINE_H, (prefix + chunk) if not first else chunk,
                         new_x="LMARGIN", new_y="NEXT")
                first = False

    pdf.output(out_pdf)
    print(f"Wrote {out_pdf}: {len(files)} files, {pdf.pages_count} pages")

if __name__ == "__main__":
    main()