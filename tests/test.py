from yoga import box

row = box(flex_direction="row")
col = box(flex_direction="column")

cell = box(width=90, height=90, margin=5, flex_grow=1)

tree = row(width=610)(
    box(id="cal", flex_grow=1, flex_wrap="wrap", padding=5, justify_content="stretch")(
        cell()
        for _ in range(31)
    ),
    box(
        id="legend",
        position="absolute",
        width=200,
        height=200,
        right=25,
        top=25,
    ),
).layout()

# Just display

from domtree.svg import svg, g, rect, text

print(
    svg(width=tree.width, height=tree.height)(
        g(name=name)(
            rect(
                fill="rgba(0,0,0,0.1)",
                stroke="rgba(0,0,0,0.4)",
                stroke_width=".3",
                x=box.left,
                y=box.top,
                height=box.height,
                width=box.width,
            ),
            text(
                x=box.x(0.5),
                y=box.y(0.5),
                text_anchor="middle",
                font_size=10,
                alignment_baseline="middle",
            )(name),
        )
        for name, box in tree.items()
    )
)
