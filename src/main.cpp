#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <optional>
#include "sources/Yoga.h"

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

namespace py = pybind11;

YGAlign alignDeserialize(const std::string &name)
{
    if (name == "auto")
        return YGAlignAuto;
    if (name == "baseline")
        return YGAlignBaseline;
    if (name == "flex-start")
        return YGAlignFlexStart;
    if (name == "flex-end")
        return YGAlignFlexEnd;
    if (name == "center")
        return YGAlignCenter;
    if (name == "stretch")
        return YGAlignStretch;
    if (name == "space-around")
        return YGAlignSpaceAround;
    if (name == "space-between")
        return YGAlignSpaceBetween;

    return YGAlignAuto;
}
YGDirection directionDeserialize(const std::string &name)
{
    if (name == "inherit")
        return YGDirectionInherit;
    if (name == "ltr")
        return YGDirectionLTR;
    if (name == "rtl")
        return YGDirectionRTL;

    return YGDirectionInherit;
}
YGFlexDirection flexDirectionDeserialize(const std::string &name)
{
    if (name == "column")
        return YGFlexDirectionColumn;
    if (name == "column-reverse")
        return YGFlexDirectionColumnReverse;
    if (name == "row")
        return YGFlexDirectionRow;
    if (name == "row-reverse")
        return YGFlexDirectionRowReverse;

    return YGFlexDirectionColumn;
}
YGWrap wrapDeserialize(const std::string &name)
{
    if (name == "no-wrap")
        return YGWrapNoWrap;
    if (name == "wrap")
        return YGWrapWrap;
    if (name == "wrap-reverse")
        return YGWrapWrapReverse;

    return YGWrapNoWrap;
}
YGJustify justifyDeserialize(const std::string &name)
{
    if (name == "flex-start")
        return YGJustifyFlexStart;
    if (name == "center")
        return YGJustifyCenter;
    if (name == "flex-end")
        return YGJustifyFlexEnd;
    if (name == "space-between")
        return YGJustifySpaceBetween;
    if (name == "space-around")
        return YGJustifySpaceAround;
    if (name == "space-evenly")
        return YGJustifySpaceEvenly;

    return YGJustifyFlexStart;
}
YGOverflow overflowDeserialize(const std::string &name)
{
    if (name == "visible")
        return YGOverflowVisible;
    if (name == "hidden")
        return YGOverflowHidden;
    if (name == "scroll")
        return YGOverflowScroll;

    return YGOverflowVisible;
}
YGPositionType positionTypeDeserialize(const std::string &name)
{
    if (name == "static")
        return YGPositionTypeStatic;
    if (name == "relative")
        return YGPositionTypeRelative;
    if (name == "absolute")
        return YGPositionTypeAbsolute;

    return YGPositionTypeStatic;
}
YGPrintOptions printOptionsDeserialize(const std::string &name)
{
    if (name == "layout")
        return YGPrintOptionsLayout;
    if (name == "style")
        return YGPrintOptionsStyle;
    if (name == "children")
        return YGPrintOptionsChildren;

    return YGPrintOptionsLayout;
}
YGUnit unitDeserialize(const std::string &name)
{
    if (name == "undefined")
        return YGUnitUndefined;
    if (name == "point")
        return YGUnitPoint;
    if (name == "percent")
        return YGUnitPercent;
    if (name == "auto")
        return YGUnitAuto;

    return YGUnitUndefined;
}
YGEdge edgeDeserialize(const std::string &name)
{
    if (name == "left")
        return YGEdgeLeft;
    if (name == "top")
        return YGEdgeTop;
    if (name == "right")
        return YGEdgeRight;
    if (name == "bottom")
        return YGEdgeBottom;
    if (name == "start")
        return YGEdgeStart;
    if (name == "end")
        return YGEdgeEnd;
    if (name == "horizontal")
        return YGEdgeHorizontal;
    if (name == "vertical")
        return YGEdgeVertical;
    if (name == "all")
        return YGEdgeAll;

    return YGEdgeAll;
}
YGExperimentalFeature experimentalFeatureDeserialize(const std::string &name)
{
    if (name == "web-flex-basis")
        return YGExperimentalFeatureWebFlexBasis;

    return YGExperimentalFeatureWebFlexBasis;
}
YGDisplay displayDeserialize(const std::string &name)
{
    if (name == "flex")
        return YGDisplayFlex;
    if (name == "none")
        return YGDisplayNone;

    return YGDisplayFlex;
}


YGNodeRef makeNode(YGConfigRef config, py::dict &tree, std::vector<YGNodeRef> &allocatedNodes)
{
    auto node = YGNodeNewWithConfig(config);
    
    if (tree.contains("width")) {
        auto raw = tree["width"];
        if (py::isinstance<py::int_>(raw) || py::isinstance<py::float_>(raw)) {
            YGNodeStyleSetWidth(node, raw.cast<float>());
        } else if (py::isinstance<py::str>(raw)) {
            auto text = raw.cast<std::string>();
            if (text == "auto") {
                YGNodeStyleSetWidthAuto(node);
            } else {
                auto ss = std::istringstream(text);
                float value;
                std::string unit;
                ss >> value >> unit;
                if (unit == "%") {
                    YGNodeStyleSetWidthPercent(node, value);
                } else {
                    YGNodeStyleSetWidth(node, value);
                }
            }
        }
    }

    if (tree.contains("height")) {
        auto raw = tree["height"];
        if (py::isinstance<py::int_>(raw) || py::isinstance<py::float_>(raw)) {
            YGNodeStyleSetHeight(node, raw.cast<float>());
        } else if (py::isinstance<py::str>(raw)) {
            auto text = raw.cast<std::string>();
            if (text == "auto") {
                YGNodeStyleSetHeightAuto(node);
            } else {
                auto ss = std::istringstream(text);
                float value;
                std::string unit;
                ss >> value >> unit;
                if (unit == "%") {
                    YGNodeStyleSetHeightPercent(node, value);
                } else {
                    YGNodeStyleSetHeight(node, value);
                }
            }
        }
    }

    allocatedNodes.push_back(node);
    if (tree.contains("children"))
    {
        unsigned i = 0;
        for (auto &child : tree["children"])
        {
            auto ctree = child.cast<py::dict>();
            auto cnode = makeNode(config, ctree, allocatedNodes);
            YGNodeInsertChild(node, cnode, i);
            i++;
        }
    }
    return node;
}

void writeComputedLayout(py::dict &tree, std::vector<YGNodeRef>& nodes, std::size_t* position) {
    auto node = nodes[*position];
    auto layout = py::dict();
    layout["left"] = YGNodeLayoutGetLeft(node);
    layout["top"] = YGNodeLayoutGetTop(node);
    layout["height"] = YGNodeLayoutGetHeight(node);
    layout["width"] = YGNodeLayoutGetWidth(node);
    tree["layout"] = layout;
    (*position)++;
    if (tree.contains("children"))
    {
        for (auto &child : tree["children"])
        {
            auto d = child.cast<py::dict>();
            writeComputedLayout(d, nodes, position);
        }
    }
}

void computeLayout(py::dict &tree, std::optional<float> width, std::optional<float> height)
{
    YGConfigRef config = YGConfigNew();
    YGConfigSetUseWebDefaults(config, true);

    std::vector<YGNodeRef> allocatedNodes;

    auto root = makeNode(config, tree, allocatedNodes);

    YGNodeCalculateLayout(
        root, 
        width.has_value() ? width.value() : YGUndefined, 
        height.has_value() ? height.value() : YGUndefined, 
        YGDirectionLTR
    );

    std::size_t position = 0;
    writeComputedLayout(tree, allocatedNodes, &position);

    for (YGNodeRef node : allocatedNodes)
    {
        YGNodeFree(node);
    }
}

PYBIND11_MODULE(_core, m)
{
    m.def("compute_layout", &computeLayout, py::arg("tree"), py::arg("width") = std::nullopt, py::arg("height") = std::nullopt);

#ifdef VERSION_INFO
    m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
    m.attr("__version__") = "dev";
#endif
}
