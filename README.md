# FlexBox layout with Yoga in Python

FlexBox is a very neat layout engine available in all browsers. You can play around with it in this [Playground](https://yogalayout.com/playground) to see which properties you need to get the desired results.

This is how to use it in Python with this project:

```python
from yoga import div, layout

row = div(flex_direction="row")
col = div(flex_direction="column")

cell = div(width=90, height=90, margin=5)

tree = layout(width=710)(
        div(id="calender", flexGrow=1, flex_wrap="wrap", padding=5)(
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

# Access the layout

tree["/legend"].height
[box.width for box in tree.glob("/calender/*")]
tree["/legend].x(0.5)  # horizontal center of the legend box


# Make a simple drawing of all boxes in the layout

from domtree.svg import svg, g, rect, text

print(
    svg(width=layout.width, height=layout.height)(
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
