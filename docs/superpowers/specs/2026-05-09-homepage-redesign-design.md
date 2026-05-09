# Homepage Redesign — Design Spec

Date: 2026-05-09
Status: Implemented

## 背景

原首页使用 Jekyll `minima` 主题默认 `home` layout，效果是：
顶部 site title "A Noob Coder" + "Posts" 标题 + 文章列表。
存在三个问题：

1. **身份不对齐**：站点定位是量化金融工程师 + 金融思考 + 技术博客，但 title/about/financial 都未传达此身份。
2. **结构平铺**：无 hero、无主题分类，访客无法在首屏理解站点讲什么。
3. **`financial.md` 几乎空白**，与所宣称定位形成反差。

## 目标

- 风格：工程师卡片风（黑白 + 一种点缀色，sans-serif，简洁）。
- 首页模块（按用户选定）：Hero 身份条 / 最近文章列表 / 分主题入口卡片。
- 不破坏其它页面（about / financial / post 详情 / 404）。
- 不引入新依赖（坚持 Jekyll 3.8 + minima 2.0 + jekyll-feed）。

## 决策

| 维度 | 决策 | 理由 |
|---|---|---|
| 视觉风格 | 工程师卡片风 | 用户选定 |
| 实施路径 | 新增独立 `_layouts/home.html`，**不 extend default** | 改动局部化；其他页面继续走 minima，零回归 |
| 点缀色 | `#B7410E`（砖红） | warm，避开"科技青蓝"的俗套，呼应金融 |
| 字体栈 | 系统 sans-serif（含 PingFang SC / Noto CJK） | 零网络请求；中英文都好看 |
| 容器宽 | 860px | 单列叙事流，利于阅读 |
| 主题入口 | 3 张卡片：Quant / Engineering / About | 用首页结构倒逼后续填充 `financial.md` |
| Title | "A Noob Coder" → "Hark Huang" | 旧标题与目标身份冲突 |
| Archive | 新增 `archive.md`，permalink `/archive/` | "View all posts" 需要去处 |

## 落地清单

**新增**

- `_layouts/home.html` — 独立 layout，含 head/header/hero/themes/recent/footer + 内联 CSS（CSS variables 集中管理颜色与最大宽度）。
- `archive.md` — 全部文章按倒序列出。
- `docs/superpowers/specs/2026-05-09-homepage-redesign-design.md` — 本文档。

**修改 `_config.yml`**

- `title`: A Noob Coder → Hark Huang
- `description`: code for my dream → Quantitative Finance Engineer. Notes on C++, systems, and markets.
- 新增 `tagline`：Hero subtitle 来源
- 新增 `bio`：Hero 段落来源
- 取消注释 `github_username: harkhuang`
- 启用 `exclude:`（含 `docs/` 让 spec 不被 build 进 _site）

**未触动**

- `index.md`（继续 `layout: home`）
- `about.md` / `financial.md` / `_posts/` / `404.html`

## 已知遗留

`about.md` 仍然写着"一介草民"，`financial.md` 几乎空白——它们与新 hero 的"Quantitative Finance Engineer"身份不一致。本次未改，留给用户单独处理。

## 实施细节

### 信息架构

```
Header   : brand + nav (About / Financial / Archive / RSS)
Hero     : H1 site.title  + subtitle  + bio  + 3 outbound chips
Themes   : 3 cards (Quant→/financial/, Engineering→/archive/, About→/about/)
Recent   : "Recent" 小标题 + 6 篇最新（日期 + 标题），下方"View all posts →"
Footer   : copyright + GitHub/Email/RSS
```

### 颜色 token

```
--text:       #1a1a1a
--text-muted: #666
--bg:         #fff
--border:     #e5e5e5
--accent:     #b7410e
```

### 交互

- 链接静态：去掉默认下划线，用 `border-bottom` 模拟。
- Hover：链接颜色与边框同时变 accent；卡片额外 `translateY(-2px)`。
- 主题卡 label 用大写小字 + 0.1em letter-spacing（杂志/工程师感）。

### 响应式

`max-width: 700px` 时：themes grid 退化单列；hero 字号 38→30；posts li 改为日期与标题纵向堆叠。

## 验证

- 本机无 Ruby/Jekyll 环境，未做 `jekyll build`。
- 已通过：YAML 解析（`_config.yml`）；Liquid block tag 平衡（5 开 5 闭）；`{{ }}` 27 对全平衡。
- 推送到 `origin/master` 后 GitHub Pages 会触发构建，构建失败会从 GitHub 邮件通知。

## 不做（YAGNI）

- 暗色模式
- 标签 / 分类系统
- 文章摘要 / 阅读时间
- 项目作品区（用户在 brainstorming 中未选）
- 切换主题（minima → 自定义） — 当前方案以最小成本达到效果
