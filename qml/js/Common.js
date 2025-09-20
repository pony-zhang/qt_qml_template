// Common QML helper functions - only what's hard to do in pure QML

// Math utilities that are cumbersome in QML expressions
function clamp(value, min, max) {
    return Math.min(Math.max(value, min), max)
}

function lerp(start, end, t) {
    return start + (end - start) * t
}

function distance(x1, y1, x2, y2) {
    return Math.sqrt(Math.pow(x2 - x1, 2) + Math.pow(y2 - y1, 2))
}

function map(value, inMin, inMax, outMin, outMax) {
    return (value - inMin) * (outMax - outMin) / (inMax - inMin) + outMin
}

// Data formatting utilities
function formatFileSize(bytes) {
    if (bytes === 0) return "0 B"
    const k = 1024
    const sizes = ["B", "KB", "MB", "GB", "TB"]
    const i = Math.floor(Math.log(bytes) / Math.log(k))
    return parseFloat((bytes / Math.pow(k, i)).toFixed(2)) + " " + sizes[i]
}

function formatDuration(seconds) {
    const hours = Math.floor(seconds / 3600)
    const minutes = Math.floor((seconds % 3600) / 60)
    const secs = Math.floor(seconds % 60)

    if (hours > 0) {
        return `${hours}:${minutes.toString().padStart(2, '0')}:${secs.toString().padStart(2, '0')}`
    }
    return `${minutes}:${secs.toString().padStart(2, '0')}`
}

// String utilities
function truncate(text, maxLength) {
    return text.length <= maxLength ? text : text.substring(0, maxLength - 3) + "..."
}

function generateId() {
    return Math.random().toString(36).substring(2, 11)
}

// Validation utilities
function isValidEmail(email) {
    return /^[^\s@]+@[^\s@]+\.[^\s@]+$/.test(email)
}

function isValidUrl(url) {
    try {
        new URL(url)
        return true
    } catch {
        return false
    }
}

// Color utilities (hex/rgb conversion)
function hexToRgb(hex) {
    const result = /^#?([a-f\d]{2})([a-f\d]{2})([a-f\d]{2})$/i.exec(hex)
    return result ? {
        r: parseInt(result[1], 16),
        g: parseInt(result[2], 16),
        b: parseInt(result[3], 16)
    } : null
}

function rgbToHex(r, g, b) {
    return "#" + ((1 << 24) + (r << 16) + (g << 8) + b).toString(16).slice(1)
}

// Array utilities that are missing in QML
function unique(array) {
    return [...new Set(array)]
}

function sum(array) {
    return array.reduce((a, b) => a + b, 0)
}

function average(array) {
    return array.length > 0 ? sum(array) / array.length : 0
}

// Date utilities
function formatDate(date, format = 'YYYY-MM-DD') {
    const d = new Date(date)
    const year = d.getFullYear()
    const month = String(d.getMonth() + 1).padStart(2, '0')
    const day = String(d.getDate()).padStart(2, '0')

    return format
        .replace('YYYY', year)
        .replace('MM', month)
        .replace('DD', day)
}

function timeAgo(date) {
    const seconds = Math.floor((new Date() - new Date(date)) / 1000)

    if (seconds < 60) return 'just now'
    if (seconds < 3600) return Math.floor(seconds / 60) + ' minutes ago'
    if (seconds < 86400) return Math.floor(seconds / 3600) + ' hours ago'
    if (seconds < 2592000) return Math.floor(seconds / 86400) + ' days ago'

    return formatDate(date)
}