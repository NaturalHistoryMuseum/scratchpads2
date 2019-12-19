"use strict";

var gulp = require("gulp");
var sass = require("gulp-sass");
var sourcemaps = require("gulp-sourcemaps");
var autoprefixer = require("gulp-autoprefixer");

gulp.task("sass:prod", function () {
  return gulp.src("./sass/*.scss")
    .pipe(sass().on("error", sass.logError))
    .pipe(autoprefixer())
    .pipe(gulp.dest("./css"));
});

gulp.task("sass:dev", function () {
  return gulp.src("./sass/*.scss")
    .pipe(sourcemaps.init())
    .pipe(sass().on("error", sass.logError))
    .pipe(autoprefixer())
    .pipe(sourcemaps.write("."))
    .pipe(gulp.dest("./css"));
});

gulp.task("sass:watch", function () {
  return gulp.watch("./sass/**/*.scss", gulp.series("sass:dev"));
});

gulp.task("default", gulp.series("sass:dev", "sass:watch"));
