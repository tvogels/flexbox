# FlexBox layout with Yoga in Python

Demo: 

```python
from yoga import div, layout

row = div(flex_direction="row")
col = div(flex_direction="column")

cell = div(width=90, height=90, margin=5)

tree = layout(
    row(width=710)(
        div(id="cal", flexGrow=1, flex_wrap="wrap", padding=5)(
            cell()
            for _ in range(31)
        ),
        div(
            id="legend",
            position="absolute",
            width=200,
            height=200,
            right=25,
            top=25,
        ),
    )
)


# Just display

from domtree.svg import svg, g, rect, text

print(
    svg(width=tree["/"].width, height=tree["/"].height)(
        g(name=name)(
            rect(
                fill="rgba(0,0,0,0.2)",
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
```