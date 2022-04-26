from yoga import div, layout

row = div(flexDirection="row", padding=2)
col = div(flexDirection="column", padding=2)

tree = layout(
    row(width=300, height=150, justifyItems="stretch")(
        div(width=50, height=50, id="left"),
        col(width=50, height=50, margin=10, id="center")(
            div(flexGrow=1),
            div(flexGrow=1),
        ),
        div(flexGrow=1, padding="2%")(
            div(flexGrow=1, marginRight="2%"),
            div(flexGrow=1),
        ),
    )
)

from domtree.svg import svg, g, rect, text

redbox = rect(fill="rgba(0,0,0,0.2)", stroke="rgba(0,0,0,0.4)", stroke_width=".3")

print(svg(width=tree[""].width, height=tree[""].height)(
    g(redbox(x=box.left, y=box.top, height=box.height, width=box.width) for name, box in tree.items()),
    g(text(x=box.x(0.5), y=box.y(0.5), text_anchor="middle", font_size=5)(name) for name, box in tree.items())
))