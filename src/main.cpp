#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <iostream>
#include <sstream>
#include <string>
#include <regex>
#include <vector>
#include <optional>
#include <stdexcept>
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

YGNodeRef makeNode(YGConfigRef config, py::object &tree, std::vector<YGNodeRef> &allocatedNodes)
{
    auto node = YGNodeNewWithConfig(config);

    std::regex lengthRegex("([\\d.]+)(px|pt|%)?", std::regex_constants::ECMAScript);

    auto attributes = tree.attr("attributes");

    if (attributes.contains("width"))
    {
        auto raw = attributes["width"];
        if (py::isinstance<py::int_>(raw) || py::isinstance<py::float_>(raw))
        {
            YGNodeStyleSetWidth(node, raw.cast<float>());
        }
        else if (py::isinstance<py::str>(raw))
        {
            auto text = raw.cast<std::string>();
            if (text == "auto")
            {
                YGNodeStyleSetWidthAuto(node);
            }
            else
            {
                std::smatch m;
                if (!std::regex_match(text, m, lengthRegex))
                {
                    throw std::invalid_argument("Illegal value in padding");
                }
                auto value = std::stof(m[1]);
                std::string unit = m[2];
                if (unit == "%")
                {
                    YGNodeStyleSetWidthPercent(node, value);
                }
                else
                {
                    YGNodeStyleSetWidth(node, value);
                }
            }
        }
    }

    if (attributes.contains("max_width"))
    {
        auto raw = attributes["max_width"];
        if (py::isinstance<py::int_>(raw) || py::isinstance<py::float_>(raw))
        {
            YGNodeStyleSetMaxWidth(node, raw.cast<float>());
        }
        else if (py::isinstance<py::str>(raw))
        {
            auto text = raw.cast<std::string>();
            std::smatch m;
            if (!std::regex_match(text, m, lengthRegex))
            {
                throw std::invalid_argument("Illegal value in padding");
            }
            auto value = std::stof(m[1]);
            std::string unit = m[2];
            if (unit == "%")
            {
                YGNodeStyleSetMaxWidthPercent(node, value);
            }
            else
            {
                YGNodeStyleSetMaxWidth(node, value);
            }
        }
    }

    if (attributes.contains("min_width"))
    {
        auto raw = attributes["min_width"];
        if (py::isinstance<py::int_>(raw) || py::isinstance<py::float_>(raw))
        {
            YGNodeStyleSetMinWidth(node, raw.cast<float>());
        }
        else if (py::isinstance<py::str>(raw))
        {
            auto text = raw.cast<std::string>();
            std::smatch m;
            if (!std::regex_match(text, m, lengthRegex))
            {
                throw std::invalid_argument("Illegal value in padding");
            }
            auto value = std::stof(m[1]);
            std::string unit = m[2];
            if (unit == "%")
            {
                YGNodeStyleSetMinWidthPercent(node, value);
            }
            else
            {
                YGNodeStyleSetMinWidth(node, value);
            }
        }
    }

    if (attributes.contains("height"))
    {
        auto raw = attributes["height"];
        if (py::isinstance<py::int_>(raw) || py::isinstance<py::float_>(raw))
        {
            YGNodeStyleSetHeight(node, raw.cast<float>());
        }
        else if (py::isinstance<py::str>(raw))
        {
            auto text = raw.cast<std::string>();
            if (text == "auto")
            {
                YGNodeStyleSetHeightAuto(node);
            }
            else
            {
                std::smatch m;
                if (!std::regex_match(text, m, lengthRegex))
                {
                    throw std::invalid_argument("Illegal value in padding");
                }
                auto value = std::stof(m[1]);
                std::string unit = m[2];
                if (unit == "%")
                {
                    YGNodeStyleSetHeightPercent(node, value);
                }
                else
                {
                    YGNodeStyleSetHeight(node, value);
                }
            }
        }
    }

    if (attributes.contains("max_height"))
    {
        auto raw = attributes["max_height"];
        if (py::isinstance<py::int_>(raw) || py::isinstance<py::float_>(raw))
        {
            YGNodeStyleSetMaxHeight(node, raw.cast<float>());
        }
        else if (py::isinstance<py::str>(raw))
        {
            auto text = raw.cast<std::string>();
            std::smatch m;
            if (!std::regex_match(text, m, lengthRegex))
            {
                throw std::invalid_argument("Illegal value in padding");
            }
            auto value = std::stof(m[1]);
            std::string unit = m[2];
            if (unit == "%")
            {
                YGNodeStyleSetMaxHeightPercent(node, value);
            }
            else
            {
                YGNodeStyleSetMaxHeight(node, value);
            }
        }
    }

    if (attributes.contains("min_height"))
    {
        auto raw = attributes["min_height"];
        if (py::isinstance<py::int_>(raw) || py::isinstance<py::float_>(raw))
        {
            YGNodeStyleSetMinHeight(node, raw.cast<float>());
        }
        else if (py::isinstance<py::str>(raw))
        {
            auto text = raw.cast<std::string>();
            auto ss = std::istringstream(text);
            float value;
            std::string unit;
            ss >> value >> unit;
            if (unit == "%")
            {
                YGNodeStyleSetMinHeightPercent(node, value);
            }
            else
            {
                YGNodeStyleSetMinHeight(node, value);
            }
        }
    }

    if (attributes.contains("flex_basis"))
    {
        auto raw = attributes["flex_basis"];
        if (py::isinstance<py::int_>(raw) || py::isinstance<py::float_>(raw))
        {
            YGNodeStyleSetFlexBasis(node, raw.cast<float>());
        }
        else if (py::isinstance<py::str>(raw))
        {
            auto text = raw.cast<std::string>();
            if (text == "auto")
            {
                YGNodeStyleSetFlexBasisAuto(node);
            }
            else
            {
                auto ss = std::istringstream(text);
                float value;
                std::string unit;
                ss >> value >> unit;
                if (unit == "%")
                {
                    YGNodeStyleSetFlexBasisPercent(node, value);
                }
                else
                {
                    YGNodeStyleSetFlexBasis(node, value);
                }
            }
        }
    }

    if (attributes.contains("flex_direction"))
    {
        auto raw = attributes["flex_direction"].cast<std::string>();
        ;
        YGNodeStyleSetFlexDirection(node, flexDirectionDeserialize(raw));
    }

    if (attributes.contains("justify_content"))
    {
        auto raw = attributes["justify_content"].cast<std::string>();
        ;
        YGNodeStyleSetJustifyContent(node, justifyDeserialize(raw));
    }

    if (attributes.contains("align_items"))
    {
        auto raw = attributes["align_items"].cast<std::string>();
        ;
        YGNodeStyleSetAlignItems(node, alignDeserialize(raw));
    }

    if (attributes.contains("align_self"))
    {
        auto raw = attributes["align_self"].cast<std::string>();
        ;
        YGNodeStyleSetAlignSelf(node, alignDeserialize(raw));
    }

    if (attributes.contains("align_content"))
    {
        auto raw = attributes["align_content"].cast<std::string>();
        ;
        YGNodeStyleSetAlignContent(node, alignDeserialize(raw));
    }

    if (attributes.contains("aspect_ratio"))
    {
        auto raw = attributes["aspect_ratio"].cast<float>();
        YGNodeStyleSetAspectRatio(node, raw);
    }

    if (attributes.contains("flex_grow"))
    {
        auto raw = attributes["flex_grow"].cast<float>();
        YGNodeStyleSetFlexGrow(node, raw);
    }

    if (attributes.contains("flex_shrink"))
    {
        auto raw = attributes["flex_shrink"].cast<float>();
        YGNodeStyleSetFlexShrink(node, raw);
    }

    if (attributes.contains("direction"))
    {
        auto raw = attributes["direction"].cast<std::string>();
        YGNodeStyleSetDirection(node, directionDeserialize(raw));
    }

    if (attributes.contains("flex_wrap"))
    {
        auto raw = attributes["flex_wrap"].cast<std::string>();
        YGNodeStyleSetFlexWrap(node, wrapDeserialize(raw));
    }

    if (attributes.contains("position"))
    {
        auto raw = attributes["position"].cast<std::string>();
        YGNodeStyleSetPositionType(node, positionTypeDeserialize(raw));
    }

    if (attributes.contains("left"))
    {
        auto raw = attributes["left"];
        YGEdge edge = YGEdgeLeft;
        if (py::isinstance<py::int_>(raw) || py::isinstance<py::float_>(raw))
        {
            YGNodeStyleSetPosition(node, edge, raw.cast<float>());
        }
        else
        {
            auto text = raw.cast<std::string>();
            std::smatch m;
            if (!std::regex_match(text, m, lengthRegex))
            {
                throw std::invalid_argument("Illegal value in padding");
            }
            auto value = std::stof(m[1]);
            std::string unit = m[2];
            if (unit == "%")
            {
                YGNodeStyleSetPositionPercent(node, edge, value);
            }
            else
            {
                YGNodeStyleSetPosition(node, edge, value);
            }
        }
    }

    if (attributes.contains("right"))
    {
        auto raw = attributes["right"];
        YGEdge edge = YGEdgeRight;
        if (py::isinstance<py::int_>(raw) || py::isinstance<py::float_>(raw))
        {
            YGNodeStyleSetPosition(node, edge, raw.cast<float>());
        }
        else
        {
            auto text = raw.cast<std::string>();
            std::smatch m;
            if (!std::regex_match(text, m, lengthRegex))
            {
                throw std::invalid_argument("Illegal value in padding");
            }
            auto value = std::stof(m[1]);
            std::string unit = m[2];
            if (unit == "%")
            {
                YGNodeStyleSetPositionPercent(node, edge, value);
            }
            else
            {
                YGNodeStyleSetPosition(node, edge, value);
            }
        }
    }
    if (attributes.contains("top"))
    {
        auto raw = attributes["top"];
        YGEdge edge = YGEdgeTop;
        if (py::isinstance<py::int_>(raw) || py::isinstance<py::float_>(raw))
        {
            YGNodeStyleSetPosition(node, edge, raw.cast<float>());
        }
        else
        {
            auto text = raw.cast<std::string>();
            std::smatch m;
            if (!std::regex_match(text, m, lengthRegex))
            {
                throw std::invalid_argument("Illegal value in padding");
            }
            auto value = std::stof(m[1]);
            std::string unit = m[2];
            if (unit == "%")
            {
                YGNodeStyleSetPositionPercent(node, edge, value);
            }
            else
            {
                YGNodeStyleSetPosition(node, edge, value);
            }
        }
    }
    if (attributes.contains("bottom"))
    {
        auto raw = attributes["bottom"];
        YGEdge edge = YGEdgeBottom;
        if (py::isinstance<py::int_>(raw) || py::isinstance<py::float_>(raw))
        {
            YGNodeStyleSetPosition(node, edge, raw.cast<float>());
        }
        else
        {
            auto text = raw.cast<std::string>();
            std::smatch m;
            if (!std::regex_match(text, m, lengthRegex))
            {
                throw std::invalid_argument("Illegal value in padding");
            }
            auto value = std::stof(m[1]);
            std::string unit = m[2];
            if (unit == "%")
            {
                YGNodeStyleSetPositionPercent(node, edge, value);
            }
            else
            {
                YGNodeStyleSetPosition(node, edge, value);
            }
        }
    }

    if (attributes.contains("margin_left"))
    {
        auto raw = attributes["margin_left"];
        auto edge = YGEdgeLeft;
        if (py::isinstance<py::int_>(raw) || py::isinstance<py::float_>(raw))
        {
            YGNodeStyleSetMargin(node, edge, raw.cast<float>());
        }
        else if (py::isinstance<py::str>(raw))
        {
            auto text = raw.cast<std::string>();
            std::smatch m;
            if (!std::regex_match(text, m, lengthRegex))
            {
                throw std::invalid_argument("Illegal value in padding");
            }
            auto value = std::stof(m[1]);
            std::string unit = m[2];
            if (unit == "%")
            {
                YGNodeStyleSetMarginPercent(node, edge, value);
            }
            else
            {
                YGNodeStyleSetMargin(node, edge, value);
            }
        }
    }

    if (attributes.contains("margin_right"))
    {
        auto raw = attributes["margin_right"];
        auto edge = YGEdgeRight;
        if (py::isinstance<py::int_>(raw) || py::isinstance<py::float_>(raw))
        {
            YGNodeStyleSetMargin(node, edge, raw.cast<float>());
        }
        else if (py::isinstance<py::str>(raw))
        {
            auto text = raw.cast<std::string>();
            std::smatch m;
            if (!std::regex_match(text, m, lengthRegex))
            {
                throw std::invalid_argument("Illegal value in padding");
            }
            auto value = std::stof(m[1]);
            std::string unit = m[2];
            if (unit == "%")
            {
                YGNodeStyleSetMarginPercent(node, edge, value);
            }
            else
            {
                YGNodeStyleSetMargin(node, edge, value);
            }
        }
    }

    if (attributes.contains("margin_top"))
    {
        auto raw = attributes["margin_top"];
        auto edge = YGEdgeTop;
        if (py::isinstance<py::int_>(raw) || py::isinstance<py::float_>(raw))
        {
            YGNodeStyleSetMargin(node, edge, raw.cast<float>());
        }
        else if (py::isinstance<py::str>(raw))
        {
            auto text = raw.cast<std::string>();
            std::smatch m;
            if (!std::regex_match(text, m, lengthRegex))
            {
                throw std::invalid_argument("Illegal value in padding");
            }
            auto value = std::stof(m[1]);
            std::string unit = m[2];
            if (unit == "%")
            {
                YGNodeStyleSetMarginPercent(node, edge, value);
            }
            else
            {
                YGNodeStyleSetMargin(node, edge, value);
            }
        }
    }

    if (attributes.contains("margin_bottom"))
    {
        auto raw = attributes["margin_bottom"];
        auto edge = YGEdgeBottom;
        if (py::isinstance<py::int_>(raw) || py::isinstance<py::float_>(raw))
        {
            YGNodeStyleSetMargin(node, edge, raw.cast<float>());
        }
        else if (py::isinstance<py::str>(raw))
        {
            auto text = raw.cast<std::string>();
            std::smatch m;
            if (!std::regex_match(text, m, lengthRegex))
            {
                throw std::invalid_argument("Illegal value in padding");
            }
            auto value = std::stof(m[1]);
            std::string unit = m[2];
            if (unit == "%")
            {
                YGNodeStyleSetMarginPercent(node, edge, value);
            }
            else
            {
                YGNodeStyleSetMargin(node, edge, value);
            }
        }
    }

    if (attributes.contains("padding_left"))
    {
        auto raw = attributes["padding_left"];
        auto edge = YGEdgeLeft;
        if (py::isinstance<py::int_>(raw) || py::isinstance<py::float_>(raw))
        {
            YGNodeStyleSetPadding(node, edge, raw.cast<float>());
        }
        else if (py::isinstance<py::str>(raw))
        {
            auto text = raw.cast<std::string>();
            std::smatch m;
            if (!std::regex_match(text, m, lengthRegex))
            {
                throw std::invalid_argument("Illegal value in padding");
            }
            auto value = std::stof(m[1]);
            std::string unit = m[2];
            if (unit == "%")
            {
                YGNodeStyleSetPaddingPercent(node, edge, value);
            }
            else
            {
                YGNodeStyleSetPadding(node, edge, value);
            }
        }
    }

    if (attributes.contains("padding_right"))
    {
        auto raw = attributes["padding_right"];
        auto edge = YGEdgeRight;
        if (py::isinstance<py::int_>(raw) || py::isinstance<py::float_>(raw))
        {
            YGNodeStyleSetPadding(node, edge, raw.cast<float>());
        }
        else if (py::isinstance<py::str>(raw))
        {
            auto text = raw.cast<std::string>();
            std::smatch m;
            if (!std::regex_match(text, m, lengthRegex))
            {
                throw std::invalid_argument("Illegal value in padding");
            }
            auto value = std::stof(m[1]);
            std::string unit = m[2];
            if (unit == "%")
            {
                YGNodeStyleSetPaddingPercent(node, edge, value);
            }
            else
            {
                YGNodeStyleSetPadding(node, edge, value);
            }
        }
    }

    if (attributes.contains("padding_top"))
    {
        auto raw = attributes["padding_top"];
        auto edge = YGEdgeTop;
        if (py::isinstance<py::int_>(raw) || py::isinstance<py::float_>(raw))
        {
            YGNodeStyleSetPadding(node, edge, raw.cast<float>());
        }
        else if (py::isinstance<py::str>(raw))
        {
            auto text = raw.cast<std::string>();
            std::smatch m;
            if (!std::regex_match(text, m, lengthRegex))
            {
                throw std::invalid_argument("Illegal value in padding");
            }
            auto value = std::stof(m[1]);
            std::string unit = m[2];
            if (unit == "%")
            {
                YGNodeStyleSetPaddingPercent(node, edge, value);
            }
            else
            {
                YGNodeStyleSetPadding(node, edge, value);
            }
        }
    }

    if (attributes.contains("padding_bottom"))
    {
        auto raw = attributes["padding_bottom"];
        auto edge = YGEdgeBottom;
        if (py::isinstance<py::int_>(raw) || py::isinstance<py::float_>(raw))
        {
            YGNodeStyleSetPadding(node, edge, raw.cast<float>());
        }
        else if (py::isinstance<py::str>(raw))
        {
            auto text = raw.cast<std::string>();
            auto ss = std::istringstream(text);
            float value;
            std::string unit;
            ss >> value >> unit;
            if (unit == "%")
            {
                YGNodeStyleSetPaddingPercent(node, edge, value);
            }
            else
            {
                YGNodeStyleSetPadding(node, edge, value);
            }
        }
    }

    if (attributes.contains("margin"))
    {
        auto raw = attributes["margin"];
        if (py::isinstance<py::int_>(raw) || py::isinstance<py::float_>(raw))
        {
            auto edge = YGEdgeAll;
            YGNodeStyleSetMargin(node, edge, raw.cast<float>());
        }
        else if (py::isinstance<py::str>(raw))
        {
            auto text = raw.cast<std::string>();

            // Split by whitespace
            auto ss = std::istringstream(text);
            std::vector<std::string> tokens; // Create vector to hold our words
            std::string buf;
            while (ss >> buf)
                tokens.push_back(buf);

            // Determine edges
            std::vector<YGEdge> edges;
            if (tokens.size() == 1)
            {
                edges.push_back(YGEdgeAll);
            }
            else if (tokens.size() == 2)
            {
                edges.push_back(YGEdgeVertical);
                edges.push_back(YGEdgeHorizontal);
            }
            else if (tokens.size() == 3)
            {
                edges.push_back(YGEdgeTop);
                edges.push_back(YGEdgeHorizontal);
                edges.push_back(YGEdgeBottom);
            }
            else if (tokens.size() == 4)
            {
                edges.push_back(YGEdgeTop);
                edges.push_back(YGEdgeRight);
                edges.push_back(YGEdgeBottom);
                edges.push_back(YGEdgeLeft);
            }
            else
            {
                throw std::invalid_argument("Wrong number of entries for margin");
            }

            std::size_t i = 0;
            for (auto edge : edges)
            {
                if (tokens[i] == "auto")
                {
                    YGNodeStyleSetMarginAuto(node, edge);
                }
                else
                {
                    std::smatch m;
                    if (!std::regex_match(tokens[i], m, lengthRegex))
                    {
                        throw std::invalid_argument("Illegal value in margin");
                    }
                    i += 1;
                    auto value = std::stof(m[1]);
                    std::string unit = m[2];
                    if (unit == "%")
                    {
                        YGNodeStyleSetMarginPercent(node, edge, value);
                    }
                    else
                    {
                        YGNodeStyleSetMargin(node, edge, value);
                    }
                }
            }
        }
    }

    if (attributes.contains("padding"))
    {
        auto raw = attributes["padding"];
        if (py::isinstance<py::int_>(raw) || py::isinstance<py::float_>(raw))
        {
            auto edge = YGEdgeAll;
            YGNodeStyleSetPadding(node, edge, raw.cast<float>());
        }
        else if (py::isinstance<py::str>(raw))
        {
            auto text = raw.cast<std::string>();

            // Split by whitespace
            auto ss = std::istringstream(text);
            std::vector<std::string> tokens; // Create vector to hold our words
            std::string buf;
            while (ss >> buf)
                tokens.push_back(buf);

            // Determine edges
            std::vector<YGEdge> edges;
            if (tokens.size() == 1)
            {
                edges.push_back(YGEdgeAll);
            }
            else if (tokens.size() == 2)
            {
                edges.push_back(YGEdgeVertical);
                edges.push_back(YGEdgeHorizontal);
            }
            else if (tokens.size() == 3)
            {
                edges.push_back(YGEdgeTop);
                edges.push_back(YGEdgeHorizontal);
                edges.push_back(YGEdgeBottom);
            }
            else if (tokens.size() == 4)
            {
                edges.push_back(YGEdgeTop);
                edges.push_back(YGEdgeRight);
                edges.push_back(YGEdgeBottom);
                edges.push_back(YGEdgeLeft);
            }
            else
            {
                throw std::invalid_argument("Wrong number of entries for padding");
            }

            std::size_t i = 0;
            for (auto edge : edges)
            {
                std::smatch m;
                if (!std::regex_match(tokens[i], m, lengthRegex))
                {
                    throw std::invalid_argument("Illegal value in padding");
                }
                i += 1;
                auto value = std::stof(m[1]);
                std::string unit = m[2];
                if (unit == "%")
                {
                    YGNodeStyleSetPaddingPercent(node, edge, value);
                }
                else
                {
                    YGNodeStyleSetPadding(node, edge, value);
                }
            }
        }
    }

    allocatedNodes.push_back(node);
    unsigned i = 0;
    for (auto &child : tree.attr("children"))
    {
        auto ctree = child.cast<py::object>();
        auto cnode = makeNode(config, ctree, allocatedNodes);
        YGNodeInsertChild(node, cnode, i);
        i++;
    }
    return node;
}

struct Layout
{
    float left;
    float top;
    float height;
    float width;
    float bottom;
    float right;
};

void writeComputedLayout(py::object &tree, std::vector<YGNodeRef> &nodes, std::size_t *position, py::dict &layouts, float offsetx, float offsety)
{
    auto node = nodes[*position];

    Layout layout{
        YGNodeLayoutGetLeft(node) + offsetx,
        YGNodeLayoutGetTop(node) + offsety,
        YGNodeLayoutGetHeight(node),
        YGNodeLayoutGetWidth(node),
    };
    (*position)++;
    layouts[tree] = layout;
    for (auto &child : tree.attr("children"))
    {
        auto d = child.cast<py::object>();
        writeComputedLayout(d, nodes, position, layouts, layout.left, layout.top);
    }
}

py::dict computeLayout(py::object &tree, std::optional<float> width, std::optional<float> height, std::string &direction)
{
    YGConfigRef config = YGConfigNew();
    YGConfigSetUseWebDefaults(config, true);

    std::vector<YGNodeRef> allocatedNodes;

    auto root = makeNode(config, tree, allocatedNodes);

    YGNodeCalculateLayout(
        root,
        width.has_value() ? *width : YGUndefined,
        height.has_value() ? *height : YGUndefined,
        directionDeserialize(direction));

    auto layouts = py::dict();

    std::size_t position = 0;
    writeComputedLayout(tree, allocatedNodes, &position, layouts, 0., 0.);

    for (YGNodeRef node : allocatedNodes)
    {
        YGNodeFree(node);
    }

    return layouts;
}

PYBIND11_MODULE(_core, m)
{
    m.def("compute_layout", &computeLayout, py::arg("tree"), py::arg("width") = std::nullopt, py::arg("height") = std::nullopt, py::arg("direction") = "ltr");
    py::class_<Layout>(m, "Layout")
        .def("__repr__",
             [](const Layout &l)
             {
                 std::stringstream ss;
                 ss << "<Layout left=" << l.left << " top=" << l.top << " width=" << l.width << " height=" << l.height << ">";
                 return ss.str();
             })
        .def_property_readonly("width", [](const Layout &l)
                               { return l.width; })
        .def_property_readonly("height", [](const Layout &l)
                               { return l.height; })
        .def_property_readonly("top", [](const Layout &l)
                               { return l.top; })
        .def_property_readonly("left", [](const Layout &l)
                               { return l.left; })
        .def_property_readonly("bottom", [](const Layout &l)
                               { return l.top + l.height; })
        .def_property_readonly("right", [](const Layout &l)
                               { return l.left + l.width; })
        .def(
            "x", [](const Layout &l, float x)
            { return x * l.width + l.left; },
            py::arg("position"))
        .def("y", [](const Layout &l, float y)
             { return (1 - y) * l.height + l.top; }),
        py::arg("position");

#ifdef VERSION_INFO
    m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
    m.attr("__version__") = "dev";
#endif
}
