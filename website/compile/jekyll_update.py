# /site/compile/jekyll_update.py

import os
import glob

import yaml

import pandas as pd
import plotly.graph_objects as go

from ..util.util import dt_str_to_slug


def jekyll_update():
    """ Updates Perf section of jekyll
    Args:
        None
    Returns:
        None, updates jekyll_site/_site
    """

    os.chdir("./website/jekyll_site")

    with open("./_config.yml") as f:
        config = yaml.safe_load(f)

    baseurl = config.get("baseurl", "").rstrip("/")

    benchmark_file = "./perf/benchmark.csv"
    flamegraph_dir = "./perf/flamegraphs"
    posts_dir = "./_posts"

    create_flamegraph_posts(flamegraph_dir, posts_dir, baseurl)
    create_benchmark_plot(benchmark_file, flamegraph_dir, "./perf/benchmark_plot.html", baseurl)


def create_flamegraph_posts(flamegraph_dir: str, posts_dir: str, baseurl: str) -> None:
    """ Creates one _posts .md file per flamegraph SVG
    Args:
        flamegraph_dir: Path to directory containing SVG flamegraphs, named e.g. '2026-05-02-17:27:04.svg'
        posts_dir: Path to Jekyll _posts directory to write .md files into
    Returns:
        None, writes .md post files to posts_dir
    """
    os.makedirs(posts_dir, exist_ok=True)

    for svg_path in sorted(glob.glob(os.path.join(flamegraph_dir, "*.svg"))):
        dt_str = os.path.basename(svg_path).replace(".svg", "")
        slug = dt_str_to_slug(dt_str)
        date_part = slug[:10]
        svg_url = f"{baseurl}/perf/flamegraphs/{dt_str}.svg"

        # Weird spacing intentional for MD injection
        post_content = f"""---
layout: flamegraph
title: "Flamegraph {dt_str}"
date: {date_part}
flamegraph_url: {svg_url}
---
"""
        post_path = os.path.join(posts_dir, f"{slug}-flamegraph.md")
        with open(post_path, "w") as f:
            f.write(post_content)

        print(f"Created post: {os.path.basename(post_path)}")


def create_benchmark_plot(csv_path: str, flamegraph_dir: str, output_path: str, baseurl: str = "") -> None:
    """ Reads benchmark CSV and writes an embeddable Plotly chart
        Clicking a dot opens that datetime's flamegraph post in a new tab
    Args:
        csv_path: Path to benchmark.csv with columns: datetime, AvgTick (ms)
        flamegraph_dir: Path to directory containing SVG flamegraphs
        output_path: Destination path for the embeddable HTML chart fragment
        baseurl: Jekyll baseurl from _config.yml, prepended to all post URLs
    Returns:
        None, writes embeddable HTML to output_path
    """

    df = pd.read_csv(csv_path)
    df["datetime"] = pd.to_datetime(df["datetime"], format="%Y-%m-%d-%H:%M:%S")

    def row_to_url(dt):
        slug = dt_str_to_slug(dt.strftime("%Y-%m-%d-%H:%M:%S"))
        # Jekyll default: /:year/:month/:day/:title/
        date_part = slug[:10] # "2026-05-02"
        title = slug[11:] + "-flamegraph" # "17-27-04-flamegraph"
        y, m, d = date_part.split("-")
        return f"{baseurl}/{y}/{m}/{d}/{title}/"

    post_urls = df["datetime"].apply(row_to_url).tolist()

    fig = go.Figure()
    fig.add_trace(go.Scatter(
        x=df["datetime"],
        y=df["AvgTick (ms)"],
        mode="lines+markers",
        name="Avg Tick",
        line=dict(color="#4f86f7", width=2),
        marker=dict(size=8, symbol="circle"),
        customdata=post_urls,
        hovertemplate=(
            "%{x|%Y-%m-%d %H:%M:%S}<br>"
            "AvgTick: %{y:.4f}s<br>"
            "<i>Click to open flamegraph</i>"
            "<extra></extra>"
        ),
    ))

    fig.update_layout(
        xaxis_title="Date",
        yaxis_title="AvgTick (ms)",
        xaxis=dict(tickformat="%Y-%m-%d %H:%M"),
        hovermode="x unified",
        paper_bgcolor="white",
        plot_bgcolor="white",
        font=dict(color="#111"),
        margin=dict(l=50, r=30, t=20, b=50),
        xaxis_gridcolor="#e5e5e5",
        yaxis_gridcolor="#e5e5e5",
        xaxis_linecolor="#e5e5e5",
        yaxis_linecolor="#e5e5e5",
    )

    # Weird spacing intentional for MD injection
    click_js = """
(function () {
    var div = document.getElementsByClassName('plotly-graph-div')[0];
    div.on('plotly_click', function (data) {
        var url = data.points[0].customdata;
        if (url) window.open(url, '_blank');
    });
    div.on('plotly_hover', function ()   { div.style.cursor = 'pointer'; });
    div.on('plotly_unhover', function () { div.style.cursor = 'default'; });
})();
"""

    fig.write_html(
        output_path,
        full_html=False,
        include_plotlyjs="cdn",
        default_width="100%",
        default_height="400px",
        post_script=click_js,
    )

    print(f"Benchmark plot written to {output_path}")